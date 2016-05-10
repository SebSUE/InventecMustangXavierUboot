/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <mmc.h>
#include <sparse_format.h>
#include <fastboot.h>
#include <part_efi.h>
#include <div64.h>
#include <gpt.h>


#define MAX_PTN 64

static fastboot_ptentry ptable[MAX_PTN];
static unsigned int pcount;
static efi_ptable gpt;


#define EFI_ENTRIES	120
#define SIZEOF_GPT	(2 + EFI_ENTRIES / 4)

/* Convert to char[2] in little endian format from the short integer */
#define SHORT_TO_LE16(x) { (x) & 0xff, ((x) >> 8) & 0xff }

/* Convert to char[4] in little endian format from the 32-bit integer */
#define INT_TO_LE32(x) \
		{ (x) & 0xff, ((x) >> 8) & 0xff, \
		((x) >> 16) & 0xff, ((x) >> 24) & 0xff }

/* Convert to char[8] in little endian format from the 64-bit integer */
#define LONG_TO_LE64(x) \
		{ (x) & 0xff, ((x) >> 8) & 0xff, \
		((x) >> 16) & 0xff, ((x) >> 24) & 0xff, \
		((x) >> 32) & 0xff, ((x) >> 40) & 0xff, \
		((x) >> 48) & 0xff, ((x) >> 56) & 0xff }

/* The number of partition entries per sector */
#define EFI_ENTRIES_PER_SECTOR	(EFI_SECTORSIZE / GPT_ENTRY_SIZE)
/*
 * The spec says that:
 * "If the size of the GUID Partition Entry Array is not an even multiple
 * of the logical block size, then any space left over in the last logical
 * block is Reserved and not covered by the Partition Entry Array CRC32 field."
 *
 * So this means that if the number of partition entries is not a multiple
 * of 4, there will be padding at the end of the disk, and the partition
 * entry array must therefore start at the beginning of a sector. So we
 * need to back up to a sector boundary when trying to find the start of
 * the array. The macro below helps us do that.
 *
 * Convert number of partitions to a whole number of sectors
 * We know that the partition is 128 bytes and the sector is 512
 * so there are 4 partitions in a sector. Round up to a multiple of 4.
 */
#define EFI_PART2SECT(numEntries)	\
((((numEntries + (EFI_ENTRIES_PER_SECTOR-1)) / EFI_ENTRIES_PER_SECTOR) \
* EFI_ENTRIES_PER_SECTOR) * sizeof(gpt_entry))

static const u8 random_uuid[16] = {
	0x3a, 0xf6, 0x33, 0x93, 0x25, 0x2f, 0x22, 0x40,
	0xab, 0x5b, 0x49, 0x54, 0xe1, 0x3a, 0x85, 0x8b,
};

static const legacy_mbr g_mbr = {
	.partition_record[0] = {
		.boot_ind = 0, /* non-bootable */
		.head = 0xFF, /* bogus CHS */
		.sector = 0xFF,
		.cyl = 0xFF,
		.sys_ind = EFI_PMBR_OSTYPE_EFI_GPT,
		.end_head = 0xFF, /* bogus CHS */
		.end_sector = 0xFF,
		.end_cyl = 0xFF,
	},
	.signature = SHORT_TO_LE16(MSDOS_MBR_SIGNATURE),
};

static const gpt_header g_gpt_header = {
	.signature = LONG_TO_LE64(GPT_HEADER_SIGNATURE),
	.revision = INT_TO_LE32(GPT_HEADER_REVISION_V1),
	.header_size = INT_TO_LE32(sizeof(gpt_header)),
	.my_lba = LONG_TO_LE64(GPT_PRIMARY_PARTITION_TABLE_LBA),
	.alternate_lba = LONG_TO_LE64(GPT_PRIMARY_PARTITION_TABLE_LBA),
	.first_usable_lba = LONG_TO_LE64((unsigned long long)SIZEOF_GPT),
	.partition_entry_lba
		= LONG_TO_LE64(GPT_PRIMARY_PARTITION_TABLE_LBA + 1),
	.num_partition_entries = INT_TO_LE32(EFI_ENTRIES),
	.sizeof_partition_entry = INT_TO_LE32(sizeof(gpt_entry)),
};

static efi_ptable gpt;

/* Convert char[8] in little endian format to the host format integer
 */
static inline unsigned long long le64_to_int(unsigned char *le64)
{
	return ((unsigned long long)le64[7] << 56) +
		((unsigned long long)le64[6] << 48) +
		((unsigned long long)le64[5] << 40) +
		((unsigned long long)le64[4] << 32) +
		((unsigned long long)le64[3] << 24) +
		((unsigned long long)le64[2] << 16) +
		((unsigned long long)le64[1] << 8) +
		(unsigned long long)le64[0];
}

static void init_legacy_mbr(legacy_mbr *mbr, u32 start_sect, u32 nr_sects)
{
	memcpy(mbr, &g_mbr, sizeof(legacy_mbr));
	memcpy(mbr->partition_record[0].start_sect, &start_sect, sizeof(u32));
	memcpy(mbr->partition_record[0].nr_sects, &nr_sects, sizeof(u32));
}

static void start_ptbl(efi_ptable *ptbl, unsigned blocks, int primary)
{
	gpt_header *hdr;
	u64 temp;

	memset(ptbl, 0, sizeof(*ptbl));

	if (primary) {
		hdr = &ptbl->primary_ptable.header;
		init_legacy_mbr(&ptbl->primary_ptable.mbr, 1, blocks);
		memcpy(hdr, &g_gpt_header, sizeof(gpt_header));
		temp = blocks - 1; /* Last Block */
		memcpy(hdr->alternate_lba, &temp, 8);
	} else {
		hdr = &ptbl->secondary_ptable.header;
		memcpy(hdr, &g_gpt_header, sizeof(gpt_header));
		temp = blocks - 1; /* Last Block */
		memcpy(hdr->my_lba, &temp, 8);
		temp = blocks - SIZEOF_GPT + 1;
		memcpy(hdr->partition_entry_lba, &temp, 8);
	}

	temp = blocks - SIZEOF_GPT;
	memcpy(hdr->last_usable_lba, &temp, 8);
	memcpy(&hdr->disk_guid, random_uuid, 16);
}

static void end_ptbl(efi_ptable *ptbl, int primary)
{
	gpt_header *hdr;
	gpt_entry *entry;
	u32 n;

	if (primary) {
		hdr = &ptbl->primary_ptable.header;
		entry = ptbl->primary_ptable.entry;
	} else {
		hdr = &ptbl->secondary_ptable.header;
		entry = ptbl->secondary_ptable.entry;
	}
	n = crc32(0, 0, 0);	/* unnecessary call */
	n = crc32(n, (void *)entry, sizeof(*entry) * EFI_ENTRIES);
	memcpy(hdr->partition_entry_array_crc32, &n, 4);

	n = crc32(0, 0, 0);	/* unnecessary call */
	n = crc32(0, (void *)hdr, sizeof(*hdr));
	memcpy(hdr->header_crc32, &n, 4);
}

static int add_ptn(efi_ptable *ptbl, u64 first, u64 last,
	const char *name, int primary)
{
	gpt_header *hdr;
	gpt_entry *entry;
	u32 n;

	if (primary) {
		hdr = &ptbl->primary_ptable.header;
		entry = ptbl->primary_ptable.entry;
	} else {
		hdr = &ptbl->secondary_ptable.header;
		entry = ptbl->secondary_ptable.entry;
	}

	if (first < SIZEOF_GPT) {
		error("%s: partition '%s' overlaps table\n", __func__, name);
		return -1;
	}

	if (last > le64_to_int(hdr->last_usable_lba)) {
		error("%s: partition '%s' does not fit\n", __func__, name);
		return -1;
	}

	for (n = 0; n < EFI_ENTRIES; n++, entry++) {
		if (le64_to_int(entry->ending_lba))
			continue;
		entry->partition_type_guid = PARTITION_BASIC_DATA_GUID;
		memcpy(&entry->unique_partition_guid, random_uuid, 16);
		memcpy(entry->starting_lba, &first, 8);
		memcpy(entry->ending_lba, &last, 8);
		for (n = 0; (n < PARTNAME_SZ) && *name; n++)
			entry->partition_name[n] = *name++;
		return 0;
	}
	error("%s: out of partition table entries\n", __func__);
	return -1;
}


/* We have observed a build error for bcm28155_ap board.
 * error is because CONFIG_KONA is defined for
 * the above board as a result cmd_fastboot.o is being
 * compiled hence mulitiple definitions.
*/

#ifndef CONFIG_KONA
void fastboot_flash_add_ptn(fastboot_ptentry *ptn)
{
	if (pcount < MAX_PTN) {
		memcpy(ptable + pcount, ptn, sizeof(struct fastboot_ptentry));
		pcount++;
	}
	printf("ERROR: pcount:%d is greater than MAX_PTN: %d", pcount, MAX_PTN);
}

void fastboot_flash_dump_ptn(void)
{
	unsigned int n;
	for (n = 0; n < pcount; n++) {
		fastboot_ptentry *ptn = ptable + n;
		printf("ptn %d name='%s' start=%d len=%llu\n",
		       n, ptn->name, ptn->start, ptn->length);
	}
}

fastboot_ptentry *fastboot_flash_find_ptn(const char *name)
{
	unsigned int n;

	for (n = 0; n < pcount; n++) {
		/* Make sure a substring is not accepted */
		if (strlen(name) == strlen(ptable[n].name)) {
			if (0 == strcmp(ptable[n].name, name))
				return ptable + n;
		}
	}
	return 0;
}

fastboot_ptentry *fastboot_flash_get_ptn(unsigned int n)
{
	if (n < pcount)
		return ptable + n;
	else
		return 0;
}

unsigned int fastboot_flash_get_ptn_count(void)
{
	return pcount;
}
#endif

static int populate_partitions(efi_ptable *ptbl, int blksz, int primary)
{
	int n;
	for (n = 0; n < fastboot_flash_get_ptn_count(); n++) {
		unsigned end_blk;
		fastboot_ptentry *p = fastboot_flash_get_ptn(n);
		end_blk = (p->start + lldiv(p->length, blksz)) - 1;
		if (!(p->flags & FASTBOOT_PTENTRY_FLAGS_GPT_ENTRY))
			continue;
		if (p->start < SIZEOF_GPT)
			continue;
		if (add_ptn(ptbl, p->start, end_blk, p->name, primary))
			return -1;
	}
	return 0;
}

int fastboot_format(const char *cmd)
{
	efi_ptable *ptbl = &gpt;
	struct mmc *mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);
	block_dev_desc_t *mmc_dev;
	int blk_cnt, ret;

	if (!mmc) {
		error("%s: no mmc devices available\n", __func__);
		return -1;
	}
	mmc_dev = mmc_get_dev(CONFIG_SYS_MMC_ENV_DEV);
	if (!mmc_dev) {
		error("%s: failed to get mmc device info\n", __func__);
		return -1;
	}

	blk_cnt = lldiv(sizeof(efi_ptable), mmc_dev->blksz);
#ifdef FASTBOOT_DUMP_PTN
	fastboot_flash_dump_ptn();
#endif

	start_ptbl(ptbl, mmc_dev->lba, 1);
	if (populate_partitions(ptbl, mmc_dev->blksz, 1) != 0) {
		error("%s: failed to construct primary GPT\n", __func__);
		return -1;
	}
	end_ptbl(ptbl, 1);

#ifndef CONFIG_SKIP_PRIMARY_GPT
	ret = mmc_dev->block_write(CONFIG_SYS_MMC_ENV_DEV, 0x0, blk_cnt,
		(void *)ptbl);
	debug("%s: %d blks write: %s\n", __func__, blk_cnt, ret == blk_cnt ?
		"OK" : "ERROR");
	if (ret != blk_cnt) {
		error("%s: failed to flash primary GPT\n", __func__);
		return -1;
	}
#endif
	start_ptbl(ptbl, mmc_dev->lba, 0);
	if (populate_partitions(ptbl, mmc_dev->blksz, 0) != 0) {
		error("%s: failed to construct secondary GPT\n", __func__);
		return -1;
	}
	end_ptbl(ptbl, 0);

	ret = mmc_dev->block_write(CONFIG_SYS_MMC_ENV_DEV,
		mmc_dev->lba - SIZEOF_GPT, blk_cnt, (void *)ptbl);
	debug("%s: %d blks write: %s\n", __func__, blk_cnt, ret == blk_cnt ?
		"OK" : "ERROR");
	if (ret != blk_cnt) {
		error("%s: failed to flash secondary GPT\n", __func__);
		return -1;
	}

	return 0;
}

/*
 * Check if the header is valid
 * Returns 1 if valid, 0 if invalid.
 */
int gpt_is_valid_hdr(efi_gpt_header_t *hdr)
{
	u32 n;
	u32 origcrc;
	u32 hdrsize = le32_to_cpu(hdr->header_size);
	/* Check the GUID Partition Table signature */
	if (le64_to_cpu(hdr->signature) != GPT_HEADER_SIGNATURE) {
		debug("%s: GPT Header signature is wrong 0x%llx != 0x%llx\n",
			__func__, le64_to_cpu(hdr->signature), GPT_HEADER_SIGNATURE);
		return 0;
	}
	/* Check the CRCs */
	origcrc = le32_to_cpu(hdr->header_crc32);	/* Save original crc */
	memset(&hdr->header_crc32, 0, sizeof(hdr->header_crc32));
	n = crc32(0, (void *)hdr, hdrsize);
	if (n != origcrc) {
		debug("%s: GPT Header CRC is wrong 0x%x != 0x%x\n",
			__func__, n, origcrc);
		return 0;
	}
	return 1;
}

/*
 * Check if the entries array is  valid
 * Returns 1 if valid, 0 if invalid.
*/
int gpt_is_valid_entries(efi_gpt_header_t *hdr, u32 num_entries,
	efi_gpt_entry_t *entry)
{
	u32 n;
	n = crc32(0, (void *)entry, sizeof(*entry) * num_entries);
	if (n != le32_to_cpu(hdr->partition_entry_array_crc32)) {
		debug("%s: GPT Entries CRC is wrong  0x%x != 0x%x\n", __func__,
			n, le32_to_cpu(hdr->partition_entry_array_crc32));
		return 0;
	}
	return 1;
}

/*
 * Check if the primary or alternate GPT table is valid
 * Returns 1 if valid, 0 if invalid.
 * Returns the number of entries and the actual starting entry for use.
 */
int gpt_is_valid_table(efi_ptable *ptbl, int primary, int blk_cnt,
	u32 *num_entriesp, efi_gpt_entry_t **entrypp)
{
	gpt_header *hdr;
	gpt_entry *entry;
	u32 num_entries;
	u32 entriesToBackup;

	if (primary) {
		debug("%s: Checking Primary\n", __func__);
		hdr = &ptbl->primary_ptable.header;
		num_entries = le32_to_cpu(hdr->num_partition_entries);
		entry = ptbl->primary_ptable.entry;
	} else {
		debug("%s: Checking Alternate\n", __func__);
		hdr = &ptbl->secondary_ptable.header;
		num_entries = le32_to_cpu(hdr->num_partition_entries);

		/* cast because we start at header and back up */
		entry = (gpt_entry *)&ptbl->secondary_ptable.header;

		/*
		* We need to back up to the correct starting entry based
		* on the actual number of partition entries in the header.
		 * If the number of entries is a multiple of 4, then we
		 * backup an integral number of sectors. But if the number
		 * of entries doesn't fit in an integral number of sectors,
		 * we have to back up so that the first entry is on a
		 * sector boundary, and we ignore the padding between the
		 * last entry and the start of the header.
		*/
		entriesToBackup = EFI_PART2SECT(num_entries)/sizeof(gpt_entry);

		entry -= entriesToBackup; /* pointer arithmetic */
	}
	*num_entriesp = num_entries;
	*entrypp = entry;
	return gpt_is_valid_hdr(hdr) && gpt_is_valid_entries(hdr, num_entries, entry);
}

/*
 * Get valid gpt table from emmc flash.
 */
int gpt_get_table(efi_ptable *ptbl, u32 *numEntriesp, efi_gpt_entry_t **entry)
{
	struct mmc *mmc;	/* mmc device */
	block_dev_desc_t *mmc_dev;	/* mmc device info */
	int blk_cnt;	/* number of blocks to read to get entire table */
	int ret;		/* return code */
	int isPrimary;	/* flag for primary or alternate identifier */

	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);
	if (!mmc) {
		error("%s: no mmc devices available\n", __func__);
		return -1;
	}

	mmc_dev = mmc_get_dev(CONFIG_SYS_MMC_ENV_DEV);
	if (!mmc_dev) {
		error("%s: failed to get mmc device info\n", __func__);
		return -1;
	}

	blk_cnt = sizeof(gpt) / mmc_dev->blksz;

#ifndef CONFIG_SKIP_PRIMARY_GPT
	/* Check primary GPT table */
	isPrimary = 1;
	ret = mmc_dev->block_read(CONFIG_SYS_MMC_ENV_DEV, 0x0, blk_cnt,
				(void *)ptbl);
	debug("%s: %d blks read: %s\n", __func__, blk_cnt,
	      ret == blk_cnt ? "OK" : "ERROR");
	if (ret != blk_cnt) {
		error("%s: failed to read primary GPT ret=%d\n", __func__, ret);
		return -1;
	}
	if (gpt_is_valid_table(ptbl, isPrimary, blk_cnt, numEntriesp, entry)) {
		debug("%s: Primary GPT valid\n", __func__);
		return 0;
	}
#endif

	/* Check alternate GPT table */
	/* Read more than is necessary for simplicity
	 * but there may be less than 120 entries */
	isPrimary = 0;
	ret = mmc_dev->block_read(CONFIG_SYS_MMC_ENV_DEV,
				mmc_dev->lba - SIZEOF_GPT, blk_cnt,
				(void *)ptbl);
	debug("%s: %d blks read: %s\n", __func__, blk_cnt,
	      ret == blk_cnt ? "OK" : "ERROR");
	if (ret != blk_cnt) {
		error("%s: failed to read alternate GPT ret=%d\n", __func__,
		      ret);
		return -1;
	}
	if (gpt_is_valid_table(ptbl, isPrimary, blk_cnt, numEntriesp, entry)) {
		debug("%s: Alternate GPT valid\n", __func__);
		return 0;
	} else {
		error("%s: cannot find valid GPT\n", __func__);
	}
	return -1;
}


/*
 * Dynamically register partitions discovered in GPT table
*/
static void register_partitions(u32 numEntries, gpt_entry *entry)
{
	int i, j;		/* loop counters */
	u64 starting_lba;	/* starting sector */
	u64 ending_lba;		/* ending sector */
	fastboot_ptentry ptn;	/* partition structure to fill in and register */

	/*
	 * For all entries, fill in the name/start/length/flags fields.
	 * Future enhancement might be to only fill in certain partitions
	 * that have one or more specific flags set. But currently we
	 * don't use the flags consistently so for now all partitions will
	 * be registered.
	 */
	for (i = 0; i < numEntries; entry++, i++) {
		/* Set the name */
		memset(ptn.name, 0, sizeof(ptn.name));
		/* Convert unicode name to simple char * name */
		for (j = 0; j < sizeof(ptn.name) - 1; j++) {
			if (entry->partition_name[j]) {
				ptn.name[j] = entry->partition_name[j];
			}
		}
		ptn.name[sizeof(ptn.name) - 1] = '\0'; /* force terminating null */

		/* Get the start/end sectors. Note the benefit of the __le64 type */
		starting_lba = le64_to_cpu(entry->starting_lba);
		ending_lba = le64_to_cpu(entry->ending_lba);

		/* Set the starting sector */
		ptn.start = starting_lba;

		/* Calculate the total length */
		ptn.length = (ending_lba + 1 - starting_lba) * EFI_SECTORSIZE;

		/*
		 * FIXME fastboot.h defines the flags as 32 bits
		 * but they are actually 64 bits.
		 * There is also C code (e.g. cmd_fastboot.c)
		 * that assumes 32 bits so I didn't change this until
		 * we can talk about it and figure out what to do.
		 *
		 * FIXME Another problem is that the ptn.name field
		 * is only 16 bytes but the EFI spec allows for 36 bytes.
		 * This should also be fixed.
		 */
		ptn.flags = (u32)(le64_to_cpu(entry->attributes.raw));

		printf("Adding: %32s, offset 0x%8.8x, size 0x%16.16llx, flags 0x%8.8x\n",
			ptn.name, ptn.start, ptn.length, ptn.flags);

		fastboot_flash_add_ptn(&ptn);
	}
}

/*
 * Function to find a valid GPT table and use it to populate the partition
 * data structure and register these with fastboot so the tables need not be
 * hardcoded in the custom board configuration files. The rationale here is
 * that customers may already have a gpt image built with a layout file, and
 * duplicating that layout file in u-boot is bad. Customers probably won't
 * run the "fastboot oem format" command either since they typically want to
 * avoid human intervention as part of the system build process.
 * Returns 0 on success, -1 on failure.
 */
int fastboot_discover_gpt_tables(void)
{
	u32 numEntries;			/* Number of partition entries */
	gpt_entry *entry;	/* partition entry */
	efi_ptable *ptbl = &gpt;	/* gpt table image in ram */
	int ret;			/* return code */

	ret = gpt_get_table(ptbl, &numEntries, &entry);
	if (ret) {
		error("%s: failed to get valid gpt table from device\n", __func__);
		return ret;
	}
	register_partitions(numEntries, entry);
	return 0;
}

/*
 * Retrieves information about the named partition.
 *
 * The address and size are in units of blocks.
 * 1 block = EFI_SECTORSIZE bytes.
 *
 * The block address and size parameter types were chosen to match the
 * arguments passed into the block_read function.
 */
int gpt_lookup_gpt_entry(const char *gpt_name, unsigned long *gpt_block_addr,
	lbaint_t *gpt_block_count)
{
	efi_ptable	*gpt;
	u32		i;
	u32		num_entries;
	gpt_entry	*gpt_entry;
	int		rc = -1;

	gpt = malloc(sizeof(*gpt));
	if (gpt == NULL) {
		printf("%s: Unable to allocate %d bytes to hold gpt table\n",
			__func__, sizeof(*gpt));
		goto out;
	}

	rc = gpt_get_table(gpt, &num_entries, &gpt_entry);
	if (rc != 0) {
		printf("%s: Failed to retrieve gpt table\n", __func__);
		goto out;
	}

	for (i = 0; i < num_entries; i++, gpt_entry++) {
		const char *cmp_name = gpt_name;
		efi_char16_t *part_name = gpt_entry->partition_name;

		/* The GPT table is stored in unicode, so we compare byte-by-byte */

		while ((*cmp_name == *part_name) && *cmp_name && *part_name) {
			cmp_name++;
			part_name++;
		}
		if (*cmp_name == *part_name) {
			u64 starting_lba;
			u64 ending_lba;

			/* We have a match */

			starting_lba = le64_to_cpu(gpt_entry->starting_lba);
			ending_lba   = le64_to_cpu(gpt_entry->ending_lba);

			*gpt_block_addr = starting_lba;
			*gpt_block_count = ending_lba - starting_lba + 1;

			rc = 0;
			goto out;
		}
	}

	/*
	 * No partition found
	 */

	rc = -1;

out:

	if (gpt)
		free(gpt);

	return rc;
}

static unsigned int add_ramdisk_uboot_header(struct fastboot_ptentry *ptn,
	unsigned int img_addr, unsigned int *img_size)
{
	unsigned int data_checksum, hdr_checksum;
	char *s;
	unsigned int ramdisk_loadaddr;
	char image_name[IH_NMLEN];

	/* prefix the image with image_header_t */
	image_header_t *hdr = ((image_header_t *)img_addr) - 1;

	memset(hdr, 0, sizeof(image_header_t));

	printf("Image starts at %p, header at %p\n", (void *)img_addr, hdr);

	s = getenv("ramdisk_loadaddr");
	if (s) {
		ramdisk_loadaddr = simple_strtoul(s, NULL, 16);
	} else {
		printf("(ramdisk_loaddr) environment not found, setting default"
				" ramdisk loaddr (0x83000000)\n");
		ramdisk_loadaddr = 0x83000000;
	}

	data_checksum = crc32(0, (const unsigned char *)img_addr, *img_size);

	strcpy(image_name, ptn->name);

	/* Build new/fake header to make uboot happy at boot time */
	image_set_magic(hdr, IH_MAGIC);
	image_set_time(hdr, 0);
	image_set_size(hdr, *img_size);
	image_set_load(hdr, ramdisk_loadaddr);
	image_set_ep(hdr, ramdisk_loadaddr);
	image_set_dcrc(hdr, data_checksum);
	image_set_os(hdr, IH_OS_LINUX);
	image_set_arch(hdr, IH_ARCH_ARM);
	image_set_type(hdr, IH_TYPE_RAMDISK);
	image_set_comp(hdr, IH_COMP_NONE);
	image_set_name(hdr, image_name);

	hdr_checksum = crc32(0, (const unsigned char *)hdr, sizeof(image_header_t));
	image_set_hcrc(hdr, hdr_checksum);

	*img_size += sizeof(image_header_t);

	return (unsigned int)hdr;
}

void emmc_flash_boot_partition(unsigned char *buffer, unsigned int size,
	char *response, const char *cmdbuf)
{
	struct mmc *mmc;

	unsigned int blk, cnt;
	unsigned int img_addr, img_size;
	unsigned int write_done = 0;
	unsigned int blksz;

	img_addr = (unsigned int)buffer;
	img_size = size;

	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc) {
		printf("mmc device not found\n");
		sprintf(response, "FAILmmc device not found");
		return;
	}

	blksz = mmc->block_dev.blksz;
	img_size = (img_size + (blksz-1)) & ~(blksz-1);
	cnt = img_size / blksz;
	blk = 0; /* GPT at the start of eMMC */

	/* Switch to the eMMC first boot partition */
	mmc_switch_part(CONFIG_SYS_MMC_ENV_DEV, 1);
	if (mmc->block_dev.block_write(CONFIG_SYS_MMC_ENV_DEV, blk, cnt,
				       (void *)img_addr) != cnt) {
		printf("Writing bootloader to first boot partition FAILED!\n");
	} else {
		printf("Writing bootloader to first boot partition DONE\n");
		write_done++;
	}

	/* Switch to the eMMC second boot partition */
	mmc_switch_part(CONFIG_SYS_MMC_ENV_DEV, 2);
	if (mmc->block_dev.block_write(CONFIG_SYS_MMC_ENV_DEV, blk, cnt,
				       (void *)img_addr) != cnt) {
		printf("Writing bootloader to second boot partition FAILED!\n");
	} else {
		printf("Writing bootloader to second boot partition DONE\n");
		write_done++;
	}

	if (!write_done)
		sprintf(response, "FAIL: Write to 'boot1' partition");
	else
		sprintf(response, "OKAY");

	/* Switch back to User data partition */
	mmc_switch_part(CONFIG_SYS_MMC_ENV_DEV, 0);

	printf("Writing bootloader DONE!\n");
}

void emmc_flash_gpt(unsigned char *buffer, unsigned int size,
	char *response, const char *cmdbuf)
{
	struct mmc *mmc;

	unsigned int blk, cnt;
	unsigned int img_addr, img_size;

	img_addr = (unsigned int)buffer;
	img_size = size;

	img_size = (img_size + (512-1)) & ~(512-1);

	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc) {
		printf("mmc device not found\n");
		sprintf(response, "FAILmmc device not found");
		return;
	}

#ifndef CONFIG_SKIP_PRIMARY_GPT
	cnt = img_size / 512;
	blk = 0; /* GPT at the start of eMMC */
#else
	cnt = img_size / 512;
	blk = (mmc->capacity - img_size) / 512; /* GPT at the end of eMMC */
#endif /* !CONFIG_SKIP_PRIMARY_GPT */

	printf("Writing GPT'\n");

	if (mmc->block_dev.block_write(CONFIG_SYS_MMC_ENV_DEV, blk, cnt,
		(void *)img_addr) != cnt) {
		printf("Writing 'gpt' FAILED!\n");
		sprintf(response, "FAIL: Write 'gpt' partition");
		return;
	}

	printf("Writing GPT DONE!\n");
	fastboot_discover_gpt_tables();
	sprintf(response, "OKAY");
}
void emmc_flash_name_image(unsigned char *buffer,
	unsigned int size, char *response, const char *cmdbuf)
{
	sparse_header_t *s_header;
	s_header = (sparse_header_t *)buffer;

	if ((s_header->magic == SPARSE_HEADER_MAGIC) &&
	    (s_header->major_version == 1)) {
		emmc_flash_sparse_image(buffer, size, response, cmdbuf);
	} else {
		emmc_flash_raw_image(buffer, size, cmdbuf + strlen("flash:"),
				     0, response);
	}
}
void emmc_flash_sparse_image(unsigned char *buffer,
	unsigned int size, char *response, const char *cmdbuf)
{
	struct mmc *mmc;

	unsigned int blk, cnt;
	unsigned int remaining_chunks;

	struct fastboot_ptentry *ptn;

	sparse_header_t *s_header;
	chunk_header_t *c_header;

	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc) {
		printf("mmc device not found\n");
		sprintf(response, "FAILmmc device not found");
		return;
	}

	ptn = fastboot_flash_find_ptn(cmdbuf + strlen("flash:"));

	if (ptn == 0) {
		printf("Partition %s does not exist\n", cmdbuf + strlen("flash:"));
		sprintf(response, "FAILpartition does not exist");
		return;
	}

	if (size > ptn->length) {
		printf("Image too large for the partition\n");
		sprintf(response, "FAILimage too large for partition");
		return;
	}

	s_header = (sparse_header_t *)buffer;

	if (((unsigned long long)s_header->blk_sz *
		(unsigned long long)s_header->total_blks) > ptn->length) {
		printf("Image too large for the partition\n");
		sprintf(response, "FAILimage too large for partition");
		return;
	}

	printf("Flashing Sparse Image\n");

	remaining_chunks = s_header->total_chunks;
	c_header = (chunk_header_t *)(buffer + s_header->file_hdr_sz);
	blk = ptn->start;

	while (remaining_chunks)
	{
		switch (c_header->chunk_type) {
		case CHUNK_TYPE_RAW:
			cnt = (c_header->chunk_sz * s_header->blk_sz) / 512;
			void *addr = (uint8_t *)c_header + s_header->chunk_hdr_sz;

			int n = mmc->block_dev.block_write(CONFIG_SYS_MMC_ENV_DEV,
						blk, cnt, addr);

			if (n != cnt) {
				printf("Write failed\n");
				sprintf(response, "FAILmmc failure prevented write");
				return;
			}
			break;

		case CHUNK_TYPE_DONT_CARE:
			/* do nothing since this is a don't care chunk */
			break;

		default:
			/* error */
			printf("Unknown chunk type\n");
			sprintf(response, "FAILunknown chunk type in sparse image");
			return;
		}

		blk += (c_header->chunk_sz * s_header->blk_sz) / 512;
		c_header = (chunk_header_t *)((uint8_t *)c_header +
			c_header->total_sz);
		remaining_chunks--;
	}

	printf("\nFlashing sparse image Finished\n");
	sprintf(response, "OKAY");
}

static int next_block = 0;
static int flashed_size = 0;
static int trunk_failed = 0;

void emmc_flash_raw_image(unsigned char *buffer,
	unsigned int size, char *part_name, int index, char *response)
{
	struct mmc *mmc;

	unsigned int blk, cnt;
	unsigned int img_addr, img_size;

	struct fastboot_ptentry *ptn;

	if (index > 0) {
		if (trunk_failed == 1) {
			printf("Previous Image trunk failed\n");
			sprintf(response, "FAILPrevious Image trunk failed");
			return;
		}
		if ((size & (512 - 1)) != 0) {
			printf("Image trunk size not 512-byte alianed\n");
			sprintf(response, "FAILImage trunk size not 512-byte alianed");
			return;
		}
	}

	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc) {
		printf("mmc device not found\n");
		sprintf(response, "FAILmmc device not found");
		return;
	}

	img_addr = (unsigned int)buffer;
	img_size = size; /* round up later */
	img_size = (img_size + (512-1)) & ~(512-1);
	cnt = img_size / 512;

	ptn = fastboot_flash_find_ptn(part_name);

	if (ptn == 0) {
		printf("Partition %s does not exist\n", part_name);
		sprintf(response, "FAILpartition does not exist");
		return;
	}

	if (index <= 0) {
		/* Whole new image or first trunk of new image */
		trunk_failed = 0;
		flashed_size = 0;
		next_block = 0;

		if (!strcmp(ptn->name, "ramdisk-recovery") ||
			!strcmp(ptn->name, "ramdisk") ||
			!strcmp(ptn->name, "ramdisk-broadcom")) {
			memmove((void *)(img_addr + sizeof(image_header_t)),
				(void *)img_addr, size);
			img_addr = add_ramdisk_uboot_header(ptn, img_addr +
				sizeof(image_header_t), &img_size);

			if (!img_addr) {
				printf("failed to add uboot header to (%s)\n", ptn->name);
				sprintf(response, "FAIL: Write '%s' partition", ptn->name);
				return;
			}
		}

		if (img_size > ptn->length) {
			printf("Image too large for the partition\n");
			sprintf(response, "FAIL image too large for partition");
			return;
		}

		blk = ptn->start;
	} else {
		if ((img_size + flashed_size) > ptn->length) {
			printf("Image trunk too large for the partition\n");
			sprintf(response, "FAIL image trunk too large for partition");
			return;
		}
		blk = next_block;
	}

	printf("Flashing Raw Image\n");
	if (index < 0)
		printf("Writing to partition '%s'\n", ptn->name);
	else
		printf("Writing to partition '%s.%d', starting at 0x%x\n",
			ptn->name, index, blk);

	if (mmc->block_dev.block_write(0, blk, cnt, (void *)img_addr) != cnt) {
		if (index < 0) {
			printf("Writing '%s' FAILED!\n", ptn->name);
			sprintf(response, "FAIL: Write '%s' partition", ptn->name);
		} else {
			trunk_failed = 1;
			flashed_size = 0;
			printf("Writing '%s.%d' FAILED!\n", ptn->name, index);
			sprintf(response, "FAIL: Write '%s.%d' partition", ptn->name, index);
		}
		return;
	}

	next_block = blk + cnt;
	flashed_size += img_size;

	if (index < 0)
		printf("\nWriting '%s' DONE!\n", ptn->name);
	else
		printf("\nWriting '%s.%d' DONE!\n", ptn->name, index);
	sprintf(response, "OKAY");
}

#ifdef CMD_EMMC
int do_writegpt(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	fastboot_format(0);
	printf("wrote GPT to eMMC\n");
	return 0;
}

U_BOOT_CMD(writegpt, 1, 1, do_writegpt,
	"writegpt - write a GPT to eMMC\n",
	""
);
#endif /* CMD_EMMC */

int do_writesparsed(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct mmc *mmc;

	unsigned int ramaddr, blk, cnt;
	unsigned int remaining_chunks;

	sparse_header_t *s_header;
	chunk_header_t *c_header;

	if (argc != 3)
		return cmd_usage(cmdtp);

	ramaddr = simple_strtoul(argv[1], NULL, 16);
	blk = simple_strtoul(argv[2], NULL, 16);

	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc) {
		printf("mmc device not found\n");
		return 0;
	}

	s_header = (sparse_header_t *)ramaddr;

	printf("Flashing Sparse Image\n");

	remaining_chunks = s_header->total_chunks;
	c_header = (chunk_header_t *)((char *)ramaddr + s_header->file_hdr_sz);

	while (remaining_chunks) {
		switch (c_header->chunk_type) {
		case CHUNK_TYPE_RAW:
			cnt = (c_header->chunk_sz * s_header->blk_sz) / 512;
			void *addr = (uint8_t *)c_header + s_header->chunk_hdr_sz;

			int n = mmc->block_dev.block_write(CONFIG_SYS_MMC_ENV_DEV,
						blk, cnt, addr);

			if (n != cnt) {
				printf("Write failed\n");
				return 0;
			}
			break;

		case CHUNK_TYPE_DONT_CARE:
			/* do nothing since this is a don't care chunk */
			break;

		default:
			/* error */
			printf("Unknown chunk type\n");
			return 0;
		}

		blk += (c_header->chunk_sz * s_header->blk_sz) / 512;
		c_header = (chunk_header_t *)((uint8_t *)c_header + c_header->total_sz);
		remaining_chunks--;
	}

	printf("Done\n");
	return 0;
}

U_BOOT_CMD(writesparsed, 3, 1, do_writesparsed,
	"trigger an expansion of a sparse image from ram into eMMC",
	"[ramaddr] [mmcblock] - write a sparsed image from ram into eMMC"
);
