/*
 * Copyright 2008 - 2009 Windriver, <www.windriver.com>
 * Author: Tom Rix <Tom.Rix@windriver.com>
 *
 * (C) Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/byteorder.h>
#include <common.h>
#include <command.h>
#include <nand.h>
#include <fastboot.h>
#include <image.h>
#include <environment.h>

#include <mmc.h>
#include <sparse_format.h>
#include <watchdog.h>

#if defined(CONFIG_CDEBUGGER)
#include <crash_dump.h>
#endif

#if defined(CONFIG_SIMLOCK)
#include <asm/kona-common/fastboot_simlock.h>
#endif

#ifdef CONFIG_USB_DEVICE
#include <usb/bcm_udc.h>
#endif

/* watdog reset call */
#ifdef CONFIG_BCM59055_WDT
extern void bcm59055_wdt_reset(void);
#endif

/* eMMC methods */
#ifdef CONFIG_CMD_EMMC
extern void emmc_flash_gpt(unsigned char *buffer,
	unsigned int size, char *response, const char *cmdbuf);
extern void emmc_flash_sparse_image(unsigned char *buffer,
	unsigned int size, char *response, const char *cmdbuf);
extern void emmc_flash_raw_image(unsigned char *buffer,
	unsigned int size, char *part_name, int index, char *response);
#endif

/* NAND methods */
#ifdef CONFIG_CMD_NAND
extern void nand_flash_raw_image(unsigned char *buffer,
	unsigned int size, char *part_name, int index, char *response);
#endif

/* Use do_reset for fastboot's 'reboot' command */
extern int do_bootm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
extern int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

/* Use do_nand for fastboot's flash commands */
extern env_t *env_ptr;

/* Use do_env_set and do_env_save to permenantly save data */
int do_env_save(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
int do_env_set(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

/* Use do_bootm and do_go for fastboot's 'boot' command */
int do_go(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

/* Forward decl */
static int rx_handler(const unsigned char *buffer, unsigned int buffer_size);
static void reset_handler(void);

#ifndef CONFIG_FASTBOOT_NO_OEM
static int fastboot_oem(const char *cmd);
#endif

static struct cmd_fastboot_interface interface =
{
	.rx_handler				= rx_handler,
	.reset_handler			= reset_handler,
	.product_name			= NULL,
	.serial_no				= NULL,
	.nand_block_size		= 0,
	.transfer_buffer		= (unsigned char *)0xffffffff,
	.transfer_buffer_size	= 0,
};

static unsigned int download_size;
static unsigned int download_bytes;
static unsigned int download_bytes_unpadded;
static unsigned int download_error;
static unsigned int continue_booting;
static unsigned int upload_size;
static unsigned int upload_bytes;
static unsigned int upload_error;

/* To support the Android-style naming of flash */
#define MAX_PTN 64
static fastboot_ptentry ptable[MAX_PTN];
static unsigned int pcount;
static int static_pcount = -1;

#if 0
static void set_env(char *var, char *val)
{
	char *setenv[4] = { "setenv", NULL, NULL, NULL, };
	setenv[1] = var;
	setenv[2] = val;
	do_env_set(NULL, 0, 3, setenv);
}
#endif

/* Helper functions to avoid duplicating string names everywhere */
static inline int memcompare(const char *cmdbuf, const char *token)
{
	return (memcmp)(cmdbuf, token, strlen(token));
}

static inline int strcompare(const char *s, const char *token)
{
	return (strncmp(s, token, strlen(token)));
}

static void reset_handler(void)
{
	/* If there was a download going on, bail */
	download_size = 0;
	download_bytes = 0;
	download_bytes_unpadded = 0;
	download_error = 0;
	continue_booting = 0;
	upload_size = 0;
	upload_bytes = 0;
	upload_error = 0;
}

#define INVALID_IDX -1
#define PART_NAME_LENGTH 10

static char cur_part[PART_NAME_LENGTH + 1];
static int cur_idx;

static void fastboot_flash_raw_image(char* response, const char* cmdbuf)
{
	char *part_name;
	char *part_idx;
	int idx = INVALID_IDX;

	part_name = (char *)cmdbuf + strlen("flash:");
	part_idx = strchr(part_name, '.');

	if (NULL != part_idx)
	{
		idx = simple_strtoul(part_idx + 1, NULL, 10);

		/* Make sure part name won't have index anymore */
		*part_idx = '\0';
		printf("Got %s partition idx=%d\n", part_name, idx);

		if (idx == 0)
		{
			strncpy(cur_part, part_name, 10);
			cur_part[PART_NAME_LENGTH] = '\0';
			cur_idx = 0;
		} else if (!strcmp(part_name, cur_part) &&
			(idx == cur_idx + 1)) {
			cur_idx = idx;
		} else {
			error("Expecting trunk(%s.%d), received trunk (%s.%d)!!\n",
				cur_part, cur_idx + 1, part_name, idx);
			sprintf(response, "FAIL wrong image trunk");
			return;
		}
	} else {
		idx = INVALID_IDX;
	}
#if defined(CONFIG_CMD_NAND)
	nand_flash_raw_image(interface.transfer_buffer, download_bytes, part_name, idx, response);
#else
	emmc_flash_raw_image(interface.transfer_buffer, download_bytes, part_name, idx, response);
#endif
}

#ifdef CONFIG_CDEBUGGER
unsigned char *ramdump;
unsigned int total_datasize;
unsigned int total_datasent;
unsigned int block_size = 512;

void show_status()
{
	static unsigned char i;

	switch (i) {
	case 0:
		printf("\ruploading..\\");
		i = 1;
		break;
	case 1:
		printf("\ruploading..|");
		i = 2;
		break;
	case 2:
		printf("\ruploading../");
		i = 3;
		break;
	case 3:
		printf("\ruploading..-");
		i = 0;
		break;
	}
}

int upload_file(const char *buffer, int size)
{
	int usb_bufsize, offset = 0;
	static unsigned int j;

	/*
	 * Maximum buffer size acceptable by fastboot_tx is 512.
	 * So we need to split and send if size is greater than 512
	 */
	while (size) {
		usb_bufsize = (size > 512) ? 512 : size;
		if ((j % 2048) == 0)/* show status once in 1MB */
			show_status();
			j++;
			fastboot_tx_status(buffer + offset , usb_bufsize);
			size -= usb_bufsize;
			offset += usb_bufsize;
	}
	return 0;
}
#endif /* CONFIG_CDEBUGGER */

static int rx_handler(const unsigned char *buffer, unsigned int buffer_size)
{
	int ret = 1;
#ifdef CONFIG_CDEBUGGER
	int size;
#endif
#ifdef CONFIG_BCM59055_WDT
	static volatile unsigned int wd=0;
#endif
	/* Use 65 instead of 64
	   null gets dropped
	   strcpy's need the extra byte */
	char response[65];

	memset((void*) response, 0, 65);

	if (download_size)
	{
		/* Something to download */

		if (buffer_size)
		{
			/* Handle possible overflow */
			unsigned int transfer_size =
				download_size - download_bytes;

			if (buffer_size < transfer_size)
				transfer_size = buffer_size;

			/* Save the data to the transfer buffer */
			memcpy(interface.transfer_buffer + download_bytes,
				buffer, transfer_size);

			download_bytes += transfer_size;

			/* Check if transfer is done */
			if (download_bytes >= download_size) {
				/* Reset global transfer variable,
				   Keep download_bytes because it will be
				   used in the next possible flashing command */
				download_size = 0;

				if (download_error) {
					/* There was an earlier error */
					sprintf(response, "ERROR");
				} else {
					/* Everything has transferred,
					   send the OK response */
					sprintf(response, "OKAY");
				}
				fastboot_tx_status(response, strlen(response));

				printf("\ndownloading of %d bytes finished\n",
					download_bytes);
			}

			/* Provide some feedback */
			if (download_bytes &&
				0 == (download_bytes % (16 *
					interface.nand_block_size)))
			{
				/* Some feeback that the
				   download is happening */
				if (download_error)
					printf("X");
				else {
					printf(".");
#ifdef CONFIG_BCM59055_WDT
					wd++;
					if (wd == 2000) {
						wd = 0;
						bcm59055_wdt_reset();
						printf("*");
					}
#endif
				}
				if (0 == (download_bytes % (80 * 16 *
					interface.nand_block_size)))
					printf("\n");

			}
		} else {
			/* Ignore empty buffers */
			printf("Warning empty download buffer\n");
			printf("Ignoring\n");
		}
		ret = 0;
	} else { /* A command */

		/* Cast to make compiler happy with string functions */
		const char *cmdbuf = (char *)buffer;

		/* Generic failed response */
		sprintf(response, "FAIL");

		if (memcompare(cmdbuf, "reboot") == 0) {
			/* reboot
			   Reboot the board. */
			sprintf(response,"OKAY");

			fastboot_tx_status(response, strlen(response));
			udelay(1000000); /* 1 sec */

			do_reset(NULL, 0, 0, NULL);

			/* This code is unreachable,
			   leave it to make the compiler happy */
			return 0;
		} else if (memcompare(cmdbuf, "getvar:") == 0) {
			/* getvar
			   Get common fastboot variables
			   Board has a chance to handle other variables */
			strcpy(response,"OKAY");

			if (!memcompare(cmdbuf + strlen("getvar:"),
				"version")) {
				strcpy(response + strlen("OKAY"), FASTBOOT_VERSION);
			} else if (!memcompare(cmdbuf + strlen("getvar:"),
				"product")) {
				if (interface.product_name)
					strcpy(response + strlen("OKAY"), interface.product_name);
			} else if (!memcompare(cmdbuf + strlen("getvar:"),
				"serialno")) {
				if (interface.serial_no)
					strcpy(response + strlen("OKAY"), interface.serial_no);
			} else if (!memcompare(cmdbuf + strlen("getvar:"),
				"downloadsize")) {
				if (interface.transfer_buffer_size)
					sprintf(response + strlen("OKAY"),
						"%08x", interface.transfer_buffer_size);
			} else {
				fastboot_getvar(cmdbuf + strlen("getvar:"),
					response + strlen("OKAY"));
			}
			ret = 0;
		} else if (memcompare(cmdbuf, "erase:") == 0) {
			/* erase
			   Erase a register flash partition
			   Board has to set up flash partitions */
			sprintf(response, "FAIL erase command not supported");
			ret = 0;
		}
#ifndef CONFIG_FASTBOOT_NO_OEM
		else if (memcompare(cmdbuf, "oem") == 0) {
			ret = fastboot_oem(cmdbuf + strlen("oem") + 1);
			if (ret)
				sprintf(response, "FAIL: Unknown oem command");
			else
				sprintf(response, "OKAY");
		}
#endif
		else if (memcompare(cmdbuf, "download:") == 0) {
			/* download
			   download something ..
			   What happens to it depends on the next command after data */

			/* save the size */
			download_size = simple_strtoul(cmdbuf + strlen("download:"),
				NULL, 16);
			/* Reset the bytes count, now it is safe */
			download_bytes = 0;
			/* Reset error */
			download_error = 0;

			debug("Starting download of %d bytes\n", download_size);

			if (0 == download_size)
			{
				/* bad user input */
				sprintf(response, "FAIL data invalid size");
			}
			else if (download_size > interface.transfer_buffer_size)
			{
				/* set download_size to 0 because this is an error */
				download_size = 0;
				sprintf(response, "FAIL data too large");
			}
			else
			{
				/* The default case, the transfer fits
				   completely in the interface buffer */
				sprintf(response, "DATA%08x", download_size);
			}
			ret = 0;
		} else if (memcompare(cmdbuf, "boot") == 0) {
			/* boot
			   boot what was downloaded

			   WARNING WARNING WARNING

			   This is not what you expect.
			   The fastboot client does its own packaging of the
			   kernel.  The layout is defined in the android header
			   file bootimage.h.  This layeout is copiedlooks like this,

			   **
			   ** +-----------------+
			   ** | boot header     | 1 page
			   ** +-----------------+
			   ** | kernel          | n pages
			   ** +-----------------+
			   ** | ramdisk         | m pages
			   ** +-----------------+
			   ** | second stage    | o pages
			   ** +-----------------+
			   **

			   We only care about the kernel.
			   So we have to jump past a page.

			   What is a page size ?
			   The fastboot client uses 2048

			   The is the default value of

			   CFG_FASTBOOT_MKBOOTIMAGE_PAGE_SIZE

			*/
			sprintf(response, "FAIL boot command not supported");
			ret = 0;
		} else if (memcompare(cmdbuf, "flash:") == 0) {
			/* flash
			   Flash what was downloaded */
			if (download_bytes) {

				/* GPT */
				if (!strcmp(cmdbuf + strlen("flash:"), "gpt"))
#if defined(CONFIG_CMD_EMMC)
				{
					emmc_flash_gpt(interface.transfer_buffer, download_bytes, response, cmdbuf);
					/* Clear out existing partitions and read new ones from the GPT */
					printf("Reading partitions from GPT\n");
					pcount = 0;
					fastboot_discover_gpt_tables();
				}
#else
					printf("No GPT partition in Flash, don't flash gpt table\n");
#endif

#ifdef CONFIG_SIMLOCK
				/* SIM lock */
				else if (!strcmp(cmdbuf + strlen("flash:"), "imei1"))
					fastboot_simlock_flash_imei1(response,
						(void*)interface.transfer_buffer,
						download_bytes);
				else if (!strcmp(cmdbuf + strlen("flash:"), "imei2"))
					fastboot_simlock_flash_imei2(response,
						(void*)interface.transfer_buffer,
						download_bytes);
				else if (!strcmp(cmdbuf + strlen("flash:"), "ckdata"))
					fastboot_simlock_flash_ckdata(response,
						(void*)interface.transfer_buffer,
						download_bytes);
#ifdef CONFIG_MMC_RPMB
				else if (!strcmp(cmdbuf + strlen("flash:"), "rpmb_key"))
					fastboot_simlock_flash_emmc_rpmb_key(response,
						(void*)interface.transfer_buffer,
						download_bytes);
#endif /* CONFIG_MMC_RPMB */
#endif /* CONFIG_SIMLOCK */

				/* Other Partitions */
				else {
					sparse_header_t *s_header =
						(sparse_header_t *)interface.transfer_buffer;

					if ((s_header->magic == SPARSE_HEADER_MAGIC) &&
						(s_header->major_version == 1))
#ifdef CONFIG_CMD_EMMC
						emmc_flash_sparse_image(
							interface.transfer_buffer, download_bytes,
							response, cmdbuf);
#else
						ret = 1;
#endif
					else
						fastboot_flash_raw_image(response, cmdbuf);
				}
			} else {
				sprintf(response, "FAIL no image downloaded");
			}
			ret = 0;
		} else if (memcompare(cmdbuf, "continue") == 0) {
			/* continue
			   Stop doing fastboot */
			sprintf(response, "OKAY");
			continue_booting = 1;
			ret = 0;
		} else if ((memcompare(cmdbuf, "upload:") == 0) ||
			(memcompare(cmdbuf, "uploadraw:") == 0)) {
			/* upload
			   Upload just the data in a partition */
			sprintf(response, "FAIL upload command not supported");
			ret = 0;
		}
#ifdef CONFIG_CDEBUGGER
		else if (strcompare("upload", cmdbuf) == 0) {
				size = (total_datasize > block_size) ? block_size :
														total_datasize ;
				upload_file(ramdump + total_datasent, size);
				total_datasent += size;
				total_datasize -= size;
				if (total_datasent == total_datasize)
						printf("Transfer completed\n");
				return 0;
		} else if (memcompare(cmdbuf, "APupload") == 0) {
				ramdump = get_ap_data(&total_datasize);
				total_datasent = 0;
				sprintf(response, "UPDATA%08x %08x", total_datasize,
														block_size);
				ret = 0;
		} else if (memcompare(cmdbuf, "CPupload") == 0) {
				ramdump = get_cp_data(&total_datasize);
				total_datasent = 0;
				sprintf(response, "UPDATA%08x %08x", total_datasize,
														block_size);
				ret = 0;
		}
#endif
		else
		{
			sprintf(response, "FAIL unknown command");
		}

		fastboot_tx_status(response, strlen(response));

	} /* End of command */

	return ret;
}

static int check_against_static_partition(struct fastboot_ptentry *ptn)
{
	int ret = 0;
	struct fastboot_ptentry *c;
	int i;

	for (i = 0; i < static_pcount; i++) {
		c = fastboot_flash_get_ptn((unsigned int) i);

		if (0 == ptn->length)
			break;

		if ((ptn->start >= c->start) &&
			(ptn->start < c->start + c->length))
			break;

		if ((ptn->start + ptn->length > c->start) &&
			(ptn->start + ptn->length <= c->start + c->length))
			break;

		if ((0 == strcmp(ptn->name, c->name)) &&
			(0 == strcmp(c->name, ptn->name)))
			break;
	}

	if (i >= static_pcount)
		ret = 1;
	return ret;
}

static unsigned long long memparse(char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */

	unsigned long ret = simple_strtoul(ptr, &endptr, 0);

	switch (*endptr) {
	case 'M':
	case 'm':
		ret <<= 10;
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

static int add_partition_from_environment(char *s, char **retptr)
{
	unsigned long size;
	unsigned long offset = 0;
	char *name;
	int name_len;
	int delim;
	unsigned int flags;
	struct fastboot_ptentry part;

	if (!s) {
		printf("Bad pointer\n");
		return 1;
	}

	size = memparse(s, &s);
	if (0 == size) {
		printf("Error:FASTBOOT size of partition is 0\n");
		return 1;
	}

	/* fetch partition name and flags */
	flags = 0; /* this is going to be a regular partition */
	delim = 0;
	/* check for offset */
	if (*s == '@') {
		s++;
		offset = memparse(s, &s);
	} else {
		printf("Error:FASTBOOT offset of partition is not given\n");
		return 1;
	}

	/* now look for name */
	if (*s == '(')
		delim = ')';

	if (delim) {
		char *p;

		name = ++s;
		p = strchr((const char *)name, delim);
		if (!p) {
			printf("Error:FASTBOOT no closing %c found in partition name\n", delim);
			return 1;
		}
		name_len = p - name;
		s = p + 1;
	} else {
		printf("Error:FASTBOOT no partition name for \'%s\'\n", s);
		return 1;
	}

	/* test for options */
	while (1) {
		if (strcompare(s, "i") == 0) {
			flags |= FASTBOOT_PTENTRY_FLAGS_WRITE_I;
			s += strlen("i");
		} else if (strcompare(s, "yaffs") == 0) {
			/* yaffs */
			flags |= FASTBOOT_PTENTRY_FLAGS_WRITE_YAFFS;
			s += strlen("yaffs");
		} else if (strcompare(s, "swecc") == 0) {
			/* swecc */
			flags |= FASTBOOT_PTENTRY_FLAGS_WRITE_SW_ECC;
			s += strlen("swecc");
		} else if (strcompare(s, "hwecc") == 0) {
			/* hwecc */
			flags |= FASTBOOT_PTENTRY_FLAGS_WRITE_HW_ECC;
			s += strlen("hwecc");
		} else {
			break;
		}
		if (strcompare(s, "|") == 0)
			s += strlen("|");
	}

	/* enter this partition (offset will be calculated later if it is zero at this point) */
	part.length = size;
	part.start = offset;
	part.flags = flags;

	if (name) {
		if (name_len >= sizeof(part.name)) {
			printf("Error:FASTBOOT partition name is too long\n");
			return 1;
		}
		strncpy(&part.name[0], name, name_len);
		/* name is not null terminated */
		part.name[name_len] = '\0';
	} else {
		printf("Error:FASTBOOT no name\n");
		return 1;
	}


	/* Check if this overlaps a static partition */
	if (check_against_static_partition(&part)) {
		printf("Adding: %s, offset 0x%8.8x, size 0x%16.16llx, flags 0x%8.8x\n",
			part.name, part.start, part.length, part.flags);
		fastboot_flash_add_ptn(&part);
	}

	/* return (updated) pointer command line string */
	*retptr = s;

	/* return partition table */
	return 0;
}

int do_fastboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 1;
	char fbparts[4096], *env;
	int check_timeout = 0;
	long timeout_seconds = -1;

#ifdef CONFIG_BCM59055_WDT
	/* reset WDT once loop enter fastboot
	 * 127sec time to start flashing
	 * next reset will happen while flashing */
	bcm59055_wdt_reset();
#endif

#if defined(CONFIG_CMD_EMMC)
	/* Use GPT table already in flash to build data structures */
	fastboot_discover_gpt_tables();
#elif defined(CONFIG_CMD_NAND)
	run_command("run mtd_mtdparts", 0);
#else
	error("No partition table!\n");
#endif

	/*
	 * Place the runtime partitions at the end of the
	 * static partitions. First save the start off so
	 * it can be saved from run to run.
	 */
	if (static_pcount >= 0) {
		/* Reset */
		pcount = static_pcount;
	} else {
		/* Save */
		static_pcount = pcount;
	}
	env = getenv("fbparts");
	if (env) {
		unsigned int len;
		len = strlen(env);
		if (len && len < 4096) {
			char *s, *e;

			memcpy(&fbparts[0], env, len + 1);
			printf("Fastboot: Adding partitions from environment\n");
			s = &fbparts[0];
			e = s + len;
			while (s < e) {
				if (add_partition_from_environment(s, &s)) {
					printf("Error:Fastboot: Abort adding partitions\n");
					/* reset back to static */
					pcount = static_pcount;
					break;
				}
				/* Skip a bunch of delimiters */
				while (s < e) {
					if ((' ' == *s) ||
						('\t' == *s) ||
						('\n' == *s) ||
						('\r' == *s) ||
						(',' == *s)) {
						s++;
					} else {
						break;
					}
				}
			}
		}
	}

	/* Time out */
	if (2 == argc) {
		long try_seconds;
		char *try_seconds_end;
		/* Check for timeout */
		try_seconds = simple_strtol(argv[1], &try_seconds_end, 10);
		if ((try_seconds_end != argv[1]) && (try_seconds >= 0)) {
			check_timeout = 1;
			timeout_seconds = try_seconds;
			printf("Fastboot inactivity timeout %ld seconds\n",
				timeout_seconds);
		}
	}
	ret = fastboot_init(&interface);
	if (ret)
	{
		error("Fastboot initialization failed\n");
		return ret;
	}

	while(1) {
#ifdef CONFIG_BCM59055_WDT
		static int cnt = 0;
#endif
		fastboot_poll();
		WATCHDOG_RESET();
#ifdef CONFIG_BCM59055_WDT
		cnt++;
		if (cnt == 1000000) {
			cnt = 0;
			bcm59055_wdt_reset();
		}
#endif
		if (continue_booting) {
			run_command(getenv("bootcmd"),0);
			ret = 1;
			break;
		}
	}

	return ret;
}

U_BOOT_CMD(
	fastboot,	2,	1,	do_fastboot,
	"fastboot- use USB Fastboot protocol\n",
	"[inactive timeout]\n"
	"    - Run as a fastboot usb device.\n"
	"    - The optional inactive timeout is the decimal seconds before\n"
	"    - the normal console resumes\n"
);

#ifndef CONFIG_FASTBOOT_NO_OEM
static int fastboot_oem(const char *cmd)
{
	if (!strcompare(cmd, "defaultenv")) {
		set_default_env(0);
		saveenv();
#ifdef CMD_EMMC
	} else if (!strcompare(cmd, "format")) {
		fastboot_format(cmd + strlen("format") + 1);
	} else if (!strcompare(cmd, "wipe")) {
		mmc_wipe(0);
#endif /* CMD_EMMC */
	} else {
		return -1;
	}

	return 0;
}
#endif /* CONFIG_FASTBOOT_NO_OEM */

/*
 * Android style flash utilties */
void fastboot_flash_add_ptn(fastboot_ptentry *ptn)
{
	if (pcount < MAX_PTN) {
		memcpy(ptable + pcount, ptn, sizeof(*ptn));
		pcount++;
	}
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

int do_addheader(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int img_addr;
	unsigned int img_size;

	unsigned int data_checksum, hdr_checksum;
	char *s;
	unsigned int ramdisk_loadaddr;

	char image_name[IH_NMLEN];

	img_addr = simple_strtoul(argv[1], NULL, 16);
	img_size = simple_strtoul(argv[2], NULL, 16);

	memmove((void *)(img_addr + sizeof(image_header_t)), (void *)img_addr, img_size);

	image_header_t *hdr = (image_header_t *)img_addr;
	img_addr += sizeof(image_header_t);

	memset(hdr, 0, sizeof(image_header_t));

	printf("Image starts at %p, header at %p\n", (void *)img_addr, hdr);

	s = getenv("ramdisk_loadaddr");
	if (s) {
		ramdisk_loadaddr = simple_strtoul(s, NULL, 16);
	} else {
		printf("(ramdisk_loaddr) environment not found, "
			"setting default ramdisk loaddr (0x83000000)\n");
		ramdisk_loadaddr = 0x83000000;
	}

	data_checksum = crc32(0, (const unsigned char *)img_addr, img_size);

	strcpy(image_name, "initramfs");

	/* Build new/fake header to make uboot happy at boot time */
	image_set_magic(hdr, IH_MAGIC);
	image_set_time(hdr, 0);
	image_set_size(hdr, img_size);
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

	return 0;
}

U_BOOT_CMD(addheader, 3, 1, do_addheader,
	"add a u-boot header to an image in place",
	"[image addr] [image size] - wrap ramdisk with u-boot header"
);

/* Board specific implementation */

void fastboot_shutdown(void)
{
}

int fastboot_is_highspeed(void)
{
	return 0;
}

int fastboot_tx_status(const char *buffer, unsigned int buffer_size)
{
	int ret = 0;
#ifdef CONFIG_USB_DEVICE
	/* This sends a packet via the bulk endpoint */
	ret = usb_send_bulk_otg((uint8_t*) buffer, buffer_size);
#endif
	return ret;
}

/* Adds a string of board specific variables */
int fastboot_getvar(const char *rx_buffer, char *tx_buffer)
{
	/* Place board specific variables here */
	return 0;
}

int fastboot_init(struct cmd_fastboot_interface *interface)
{
	int ret = 0;

#ifdef CONFIG_USB_DEVICE
	struct usb_interface_descriptor desc;

	/* shutdown */
	usb_shutdown_otg();
	interface->reset_handler();

	/* get device parameters and fill fastboot interface */
	interface->product_name =
		usb_device_strings[DEVICE_STRING_PRODUCT_INDEX];
	interface->serial_no =
		usb_device_strings[DEVICE_STRING_SERIAL_NUMBER_INDEX];
	interface->nand_block_size = 2048;
	interface->transfer_buffer =
		(unsigned char *)CFG_FASTBOOT_TRANSFER_BUFFER;
	interface->transfer_buffer_size = CFG_FASTBOOT_TRANSFER_BUFFER_SIZE;

	/* set device descriptor */
	desc.bInterfaceClass = FASTBOOT_INTERFACE_CLASS;
	desc.bInterfaceSubClass = FASTBOOT_INTERFACE_SUB_CLASS;
	desc.bInterfaceProtocol = FASTBOOT_INTERFACE_PROTOCOL;
	usb_set_interface_descriptor(&desc);

	/* initialize device */
	ret = usb_init_otg(interface->rx_handler);
#else
	printf("No Fastboot device is available.\n");
	ret = -1;
#endif
	return ret;
}

int fastboot_poll(void)
{
#ifdef CONFIG_USB_DEVICE
	usb_handle_ints_otg();
#endif
	return 0;
}
