/*
 * Copyright 2011 Attero Tech, LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Permanent Constants Partition Functionality
 * (preserved during FW update process)
 */

#include <common.h>
#include <command.h>
#include <nand.h>
#include <linux/stddef.h>
#include <search.h>
#include <errno.h>
#include <malloc.h>
#include <watchdog.h>
#include <asm/arch/sys_proto.h>
#include <const_env_common.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONST_IS_EMBEDDED)
extern uchar constants[];
constants_t *const_ptr = (constants_t *)(&constants[0]);
#elif defined(CONFIG_NAND_CONST_DST)
constants_t *const_ptr = (constants_t *)CONFIG_NAND_CONST_DST;
#else /* ! CONST_IS_EMBEDDED */
constants_t *const_ptr = 0;
#endif /* CONST_IS_EMBEDDED */

/************************************************************************
 * Default settings to be used when no valid constants is found
 */

static char default_constants[] = {
	"\0"
};

struct hsearch_data const_htab;

static uchar const_get_char_spec (int index)
{
	return ( *((uchar *)(gd->const_addr + index)) );
}


/*
 * This is called before nand_init() so we can't read NAND to
 * validate constants data.
 *
 * Mark it OK for now. const_relocate() will call our
 * relocate function which does the real validation.
 *
 * When using a NAND boot image (like sequoia_nand), the constants
 * can be embedded or attached to the U-Boot image in NAND flash.
 * This way the SPL loads not only the U-Boot image from NAND but
 * also the constants.
 */
int const_init(void)
{
#if defined(CONST_IS_EMBEDDED) || defined(CONFIG_NAND_CONST_DST)
	int crc1_ok = 0, crc2_ok = 0;
	constants_t *tmp_const1;

#ifdef CONFIG_CONST_OFFSET_REDUND
	constants_t *tmp_const2;

	tmp_const2 = (constants_t *)((ulong)const_ptr + CONFIG_CONSTANTS_SIZE);
	crc2_ok = (crc32(0, tmp_const2->data, CONST_SIZE) == tmp_const2->crc);
#endif

	tmp_const1 = const_ptr;

	crc1_ok = (crc32(0, tmp_const1->data, CONST_SIZE) == tmp_const1->crc);

	if (!crc1_ok && !crc2_ok) {
		gd->const_addr  = 0;
		gd->const_valid = 0;

		return 0;
	} else if (crc1_ok && !crc2_ok) {
		gd->const_valid = 1;
	}
#ifdef CONFIG_CONST_OFFSET_REDUND
	else if (!crc1_ok && crc2_ok) {
		gd->const_valid = 2;
	} else {
		/* both ok - check serial */
		if(tmp_const1->flags == 255 && tmp_const2->flags == 0)
			gd->const_valid = 2;
		else if(tmp_const2->flags == 255 && tmp_const1->flags == 0)
			gd->const_valid = 1;
		else if(tmp_const1->flags > tmp_const2->flags)
			gd->const_valid = 1;
		else if(tmp_const2->flags > tmp_const1->flags)
			gd->const_valid = 2;
		else /* flags are equal - almost impossible */
			gd->const_valid = 1;
	}

	if (gd->const_valid == 2)
		const_ptr = tmp_const2;
	else
#endif
	if (gd->const_valid == 1)
		const_ptr = tmp_const1;

	gd->const_addr = (ulong)const_ptr->data;

#else /* CONST_IS_EMBEDDED || CONFIG_NAND_CONST_DST */
	gd->const_addr  = (ulong)&default_constants[0];
	gd->const_valid = 1;
#endif /* CONST_IS_EMBEDDED || CONFIG_NAND_CONST_DST */

	return (0);
}

static uchar const_get_char_init (int index)
{
	/* if crc was bad, use the default constants */
	if (gd->const_valid)
		return const_get_char_spec(index);
	else
		return (uchar)default_constants[index];
}

static uchar *const_get_addr (int index)
{
	if (gd->const_valid)
		return (uchar *)(gd->const_addr + index);
	else
		return (uchar*)&default_constants[index];
}

uchar const_get_char_memory (int index)
{
	return *const_get_addr(index);
}

uchar const_get_char (int index)
{
	/* if relocated to RAM */
	if (gd->flags & GD_FLG_RELOC)
		return const_get_char_memory(index);
	else
		return const_get_char_init(index);
}

static void set_default_const(const char *s)
{
	if (sizeof(default_constants) > CONST_SIZE) {
		puts("*** Error - default constants is too large\n\n");
		return;
	}

	if (s) {
		if (*s == '!') {
			printf("*** Warning - %s, "
				"using default constants\n\n",
				s+1);
		} else {
			puts(s);
		}
	} else {
		puts("Using default constants\n\n");
	}

	if (himport_r(&const_htab, (char *)default_constants,
		    sizeof(default_constants), '\0', 0, 0, 0, NULL) == 0) {
		error("Constants import failed: errno = %d\n", errno);
	}
	gd->flags |= GD_FLG_CONST_READY;
}

#ifndef CONFIG_SPL_BUILD
/*
 * Check if CRC is valid and (if yes) import the constants.
 * Note that "buf" may or may not be aligned.
 */
static int const_import(const char *buf, int check)
{
	constants_t *ep = (constants_t *)buf;

	if (check) {
		uint32_t crc;

		memcpy(&crc, &ep->crc, sizeof(crc));

		if (crc32(0, ep->data, CONST_SIZE) != crc) {
			set_default_const("!bad CRC");
			return 0;
		}
	}

	if (himport_r(&const_htab, (char *)ep->data, CONST_SIZE, '\0', 0, 0, 0, NULL)) {
		gd->flags |= GD_FLG_CONST_READY;
		return 1;
	}

	error("Cannot import constants: errno = %d\n", errno);

	set_default_const("!import failed");

	return 0;
}
#endif

static int saveconst(void);

#ifdef CONFIG_CONST_OFFSET_REDUND
static void const_relocate_spec(void)
{
#if !defined(CONST_IS_EMBEDDED)
	int crc1_ok = 0, crc2_ok = 0;
	constants_t *ep, *tmp_const1, *tmp_const2;
    volatile uint8_t ecc_conversion = 0;

	tmp_const1 = (constants_t *)malloc(CONFIG_CONSTANTS_SIZE);
	tmp_const2 = (constants_t *)malloc(CONFIG_CONSTANTS_SIZE);

	if ((tmp_const1 == NULL) || (tmp_const2 == NULL)) {
		puts("Can't allocate buffers for constants\n");
		free(tmp_const1);
		free(tmp_const2);
		set_default_const("!malloc() failed");
		return;
	}

	if (readconst(CONFIG_CONSTANTS_OFFSET, (u_char *) tmp_const1)) {
		puts("No Valid Constants Area found\n");
#ifdef CONFIG_CONST_ECC_AUTOCORRECT
        puts("INFO: readconst() failed - Attempting ECC conversion of constants partition...\n");
		/* omap_nand_switch_ecc(NAND_ECC_HW, 2); */
        if (readconst(CONFIG_CONSTANTS_OFFSET, (u_char *) tmp_const1))
            puts("INFO: No Valid Constants Area found\n");
		/* omap_nand_switch_ecc(NAND_ECC_HW, 1); */
        crc1_ok = (crc32(0, tmp_const1->data, CONST_SIZE) == tmp_const1->crc);
        if (crc1_ok) {
            ecc_conversion = 1;
        } else {
            puts("INFO: ECC conversion of constants partition failed\n");
        }
#endif // CONFIG_CONST_ECC_AUTOCORRECT
    }

	if (readconst(CONFIG_CONST_OFFSET_REDUND, (u_char *) tmp_const2)) {
		puts("No Valid Redundant Constants Area found\n");
    }

	crc1_ok = (crc32(0, tmp_const1->data, CONST_SIZE) == tmp_const1->crc);
	crc2_ok = (crc32(0, tmp_const2->data, CONST_SIZE) == tmp_const2->crc);

#ifdef CONFIG_CONST_ECC_AUTOCORRECT
    // If CRC1 is bad, check if it is possibly an ECC conversion that
    // needs to be performed
    if (!crc1_ok) {
        puts("INFO: Constants CRC failed - Attempting ECC conversion of constants partition...\n");
        /* omap_nand_switch_ecc(NAND_ECC_HW, 2); */
        if (readconst(CONFIG_CONSTANTS_OFFSET, (u_char *) tmp_const1))
            puts("INFO: No Valid Constants Area found\n");
        /* omap_nand_switch_ecc(NAND_ECC_HW, 1); */
        crc1_ok = (crc32(0, tmp_const1->data, CONST_SIZE) == tmp_const1->crc);
        if (crc1_ok) {
            ecc_conversion = 1;
        } else {
            puts("INFO: ECC conversion of constants partition failed\n");
        }
    }
#endif // CONFIG_CONST_ECC_AUTOCORRECT

	if (!crc1_ok && !crc2_ok) {
		free(tmp_const1);
		free(tmp_const2);
		set_default_const("!bad CRC");
		return;
	} else if (crc1_ok && !crc2_ok) {
		gd->const_valid = 1;
	} else if (!crc1_ok && crc2_ok) {
		gd->const_valid = 2;
	} else {
		/* both ok - default to one */
		gd->const_valid = 1;
	}

	if (gd->const_valid == 1)
		ep = tmp_const1;
	else
		ep = tmp_const2;

	const_import((char *)ep, 0);

	free(tmp_const1);
	free(tmp_const2);

#ifdef CONFIG_CONST_ECC_AUTOCORRECT
    if (ecc_conversion) {
        if (saveconst()) {
            puts("INFO: saving constants with new ECC failed\n");
        } else {
            puts("INFO: saving constants with new ECC succeeded\n");
        }
    }
#endif // CONFIG_CONST_ECC_AUTOCORRECT

#endif /* ! CONST_IS_EMBEDDED */
}
#else /* ! CONFIG_CONST_OFFSET_REDUND */
/*
 * The legacy NAND code saved the constants in the first NAND
 * device i.e., nand_dev_desc + 0. This is also the behaviour using
 * the new NAND code.
 */
static void const_relocate_spec (void)
{
#if !defined(CONST_IS_EMBEDDED)
	int ret;
	char buf[CONFIG_CONSTANTS_SIZE];

	ret = readconst(CONFIG_CONSTANTS_OFFSET, (u_char *)buf);
	if (ret) {
		set_default_const("!readconst() failed");
		return;
	}

	const_import(buf, 1);
#endif /* ! CONST_IS_EMBEDDED */
}
#endif /* CONFIG_CONST_OFFSET_REDUND */

void const_relocate (void)
{
	if (gd->const_valid == 0) {
		show_boot_progress (-60);
		set_default_const("!bad CRC");
	} else {
		const_relocate_spec ();
	}
}

/*
 * This variable is incremented on each _do_const_set(), so it can
 * be used via get_const_id() as an indication, if the constants
 * has changed or not. So it is possible to reread a constants
 * variable only if the constants was changed ... done so for
 * example in NetInitLoop() for the u-boot environment.
 */
static int const_id = 1;

int get_const_id (void)
{
	return const_id;
}

/*
 * Match a name / name=value pair
 *
 * s1 is either a simple 'name', or a 'name=value' pair.
 * i2 is the constants index for a 'name2=value2' pair.
 * If the names match, return the index for the value2, else NULL.
 */

static int constmatch (uchar *s1, int i2)
{
	if (s1 == NULL)
		return -1;

	while (*s1 == const_get_char(i2++))
		if (*s1++ == '=')
			return(i2);
	if (*s1 == '\0' && const_get_char(i2-1) == '=')
		return(i2);
	return(-1);
}

/*
 * Command interface: print one or all constants variables
 *
 * Returns 0 in case of error, or length of printed string
 */
static int const_print(char *name)
{
	char *res = NULL;
	size_t len;

	if (name) {		/* print a single name */
		ENTRY e, *ep;

		e.key = name;
		e.data = NULL;
		hsearch_r(e, FIND, &ep, &const_htab, 0);
		if (ep == NULL)
			return 0;
		len = printf ("%s=%s\n", ep->key, ep->data);
		return len;
	}

	/* print whole list */
	len = hexport_r(&const_htab, '\n', 0, &res, 0, 0, NULL);

	if (len > 0) {
		puts(res);
		free(res);
		return len;
	}

	/* should never happen */
	return 0;
}

/*
 * Set a new constants variable,
 * or replace or delete an existing one.
 */

static int _do_const_set (int flag, int argc, char * const argv[])
{
	int   i, len;
	char  *name, *value, *s;
	ENTRY e, *ep;

	name = argv[2];

	if (strchr(name, '=')) {
		printf ("## Error: illegal character '=' in variable name \"%s\"\n", name);
		return 1;
	}

	const_id++;
	/*
	 * search if variable with this name already exists
	 */
	e.key = name;
	e.data = NULL;
	hsearch_r(e, FIND, &ep, &const_htab, 0);

	/*
	 * Some variables like "mac" and "serial#" can be set only
	 * once and cannot be deleted.
	 */
	if (ep) {		/* variable exists */
#ifndef CONFIG_CONST_OVERWRITE
		if ((strcmp (name, "serial#") == 0) ||
		    ((strcmp (name, "mac") == 0) ) ) {
			printf ("Can't overwrite \"%s\"\n", name);
			return 1;
		}
#endif
	}

	/* Delete only ? */
	if ((argc < 4) || argv[3] == NULL) {
		int rc = hdelete_r(name, &const_htab,0);
		return !rc;
	}

	/*
	 * Insert / replace new value
	 */
	for (i=3,len=0; i<argc; ++i) {
		len += strlen(argv[i]) + 1;
	}
	if ((value = malloc(len)) == NULL) {
		printf("## Can't malloc %d bytes\n", len);
		return 1;
	}
	for (i=3,s=value; i<argc; ++i) {
		char *v = argv[i];

		while ((*s++ = *v++) != '\0')
			;
		*(s-1) = ' ';
	}
	if (s != value)
		*--s = '\0';

	e.key  = name;
	e.data = value;
	hsearch_r(e, ENTER, &ep, &const_htab, 0);
	free(value);
	if (!ep) {
		printf("## Error inserting \"%s\" variable, errno=%d\n",
			name, errno);
		return 1;
	}

	return 0;
}

int setconst (char *varname, char *varvalue)
{
	char * const argv[5] = { "const", "set", varname, varvalue, NULL };
	if ((varvalue == NULL) || (varvalue[0] == '\0'))
		return _do_const_set(0, 3, argv);
	else
		return _do_const_set(0, 4, argv);
}

/*
 * Look up variable from constants for restricted C runtime env.
 */
static int getconst_f (char *name, char *buf, unsigned len)
{
	int i, nxt;

	for (i=0; const_get_char(i) != '\0'; i=nxt+1) {
		int val, n;

		for (nxt=i; const_get_char(nxt) != '\0'; ++nxt) {
			if (nxt >= CONST_SIZE) {
				return (-1);
			}
		}
		if ((val=constmatch((uchar *)name, i)) < 0)
			continue;

		/* found; copy out */
		for (n=0; n<len; ++n, ++buf) {
			if ((*buf = const_get_char(val++)) == '\0')
				return n;
		}

		if (n)
			*--buf = '\0';

		printf("const_buf too small [%d]\n", len);

		return n;
	}
	return (-1);
}

/*
 * Look up variable from constants,
 * return address of storage for that variable,
 * or NULL if not found
 */
char *getconst (char *name)
{
	if (gd->flags & GD_FLG_CONST_READY) {	/* after import into hashtable */
		ENTRY e, *ep;

		WATCHDOG_RESET();

		e.key  = name;
		e.data = NULL;
		hsearch_r(e, FIND, &ep, &const_htab, 0);

		return (ep ? ep->data : NULL);
	}

	/* restricted capabilities before import */

	if (getconst_f(name, (char *)(gd->const_buf), sizeof(gd->const_buf)) > 0)
		return (char *)(gd->const_buf);

	return NULL;
}

/*
 * The legacy NAND code saved the constants in the first NAND device i.e.,
 * nand_dev_desc + 0. This is also the behaviour using the new NAND code.
 */
int writeconst(size_t offset, u_char *buf)
{
	size_t end = CONFIG_CONSTANTS_OFFSET + CONFIG_CONSTANTS_RANGE;
	size_t amount_saved = 0;
	size_t blocksize, len;
    size_t primary_offset = CONFIG_CONSTANTS_OFFSET;

	u_char *char_ptr;

	blocksize = nand_info[0].erasesize;
	len = min(blocksize, CONFIG_CONSTANTS_SIZE);

    printf("INFO: %s -> requested offset is 0x%08x\n", __FUNCTION__, offset);
    if (CONFIG_CONSTANTS_OFFSET != offset) {
        // redundant write so need to find primary offset
        while ((nand_block_isbad(&nand_info[0], primary_offset)) &&
               (primary_offset < end)) {
            printf("INFO: %s -> offset 0x%08x is bad\n",
                   __FUNCTION__, primary_offset);
            primary_offset += blocksize;
        }
        // redundant offset is adjusted based on the
        // valid primary offset
        offset = primary_offset + (offset - CONFIG_CONSTANTS_OFFSET);
        printf("INFO: %s -> redundant offset is 0x%08x\n", __FUNCTION__, offset);
    }

	while (amount_saved < CONFIG_CONSTANTS_SIZE && offset < end) {
		if (nand_block_isbad(&nand_info[0], offset)) {
            printf("INFO: %s -> offset 0x%08x is bad\n",
                   __FUNCTION__, offset);
			offset += blocksize;
		} else {
			char_ptr = &buf[amount_saved];
			if (nand_write(&nand_info[0], offset, &len,
					char_ptr)) {
                printf("ERROR: %s -> NAND write failed for offset 0x%08x...\n",
                       __FUNCTION__, offset);
				return 1;
            } else {
                printf("INFO: %s -> 0x%08x bytes written to offset 0x%08x\n",
                       __FUNCTION__, len, offset);
            }
			offset += blocksize;
			amount_saved += len;
		}
	}
	if (amount_saved != CONFIG_CONSTANTS_SIZE) {
        printf("ERROR: %s -> Amount saved(0x%08x) != constants size(0x%08x...\n",
               __FUNCTION__, amount_saved, CONFIG_CONSTANTS_SIZE);
		return 1;
    }

	return 0;
}

#ifdef CONFIG_CONST_OFFSET_REDUND

static int saveconst(void)
{
	constants_t const_new;
	ssize_t	    len;
	char	    *res;
	int	        ret1 = 0;
	int	        ret2 = 0;
	nand_erase_options_t nand_erase_options;

	nand_erase_options.length = CONFIG_CONSTANTS_RANGE;
	nand_erase_options.quiet = 0;
	nand_erase_options.jffs2 = 0;
	nand_erase_options.scrub = 0;

	if (CONFIG_CONSTANTS_RANGE < CONFIG_CONSTANTS_SIZE)
		return 1;

	res = (char *)&const_new.data;
	len = hexport_r(&const_htab, '\0', 0, &res, CONST_SIZE, 0, NULL);
	if (len < 0) {
		error("Cannot export constants: errno = %d\n", errno);
		return 1;
	}
	const_new.crc   = crc32(0, const_new.data, CONST_SIZE);

    puts("Erasing constants NAND partition...\n");
    nand_erase_options.offset = CONFIG_CONSTANTS_OFFSET;
    if (nand_erase_opts(&nand_info[0], &nand_erase_options))
        return 1;

    // write redundant constants
    puts("Writing redundant constants to NAND... ");
    ret2 = writeconst(CONFIG_CONST_OFFSET_REDUND,
                      (u_char *)&const_new);
	if (ret2) {
		puts("FAILED!\n");
	} else {
	    gd->const_valid = 2;
    }

    // write main constants
    puts("Writing constants to NAND... ");
    ret1 = writeconst(CONFIG_CONSTANTS_OFFSET,
	                  (u_char *)&const_new);
	if (ret1) {
		puts("FAILED!\n");
	} else {
	    gd->const_valid = 1;
    }

	if (ret1 || ret2) {
		return 1;
	}

	puts("done\n");

	return 0;
}
#else /* ! CONFIG_CONST_OFFSET_REDUND */
static int saveconst(void)
{
	int         ret = 0;
	constants_t const_new;
	ssize_t     len;
	char        *res;
	nand_erase_options_t nand_erase_options;

	nand_erase_options.length = CONFIG_CONSTANTS_RANGE;
	nand_erase_options.quiet = 0;
	nand_erase_options.jffs2 = 0;
	nand_erase_options.scrub = 0;
	nand_erase_options.offset = CONFIG_CONSTANTS_OFFSET;

	if (CONFIG_CONSTANTS_RANGE < CONFIG_CONSTANTS_SIZE)
		return 1;

	res = (char *)&const_new.data;
	len = hexport_r(&const_htab, '\0', 0, &res, CONST_SIZE, 0, NULL);
	if (len < 0) {
		error("Cannot export constants: errno = %d\n", errno);
		return 1;
	}
	const_new.crc   = crc32(0, const_new.data, CONST_SIZE);

	puts("Erasing Nand...\n");
	if (nand_erase_opts(&nand_info[0], &nand_erase_options))
		return 1;

	puts("Writing to Nand... ");
	if (writeconst(CONFIG_CONSTANTS_OFFSET, (u_char *)&const_new)) {
		puts("FAILED!\n");
		return 1;
	}

	puts("done\n");
	return ret;
}
#endif /* CONFIG_CONST_OFFSET_REDUND */

int readconst(size_t offset, u_char * buf)
{
	size_t end = CONFIG_CONSTANTS_OFFSET + CONFIG_CONSTANTS_RANGE;
	size_t amount_loaded = 0;
	size_t blocksize, len;
    size_t primary_offset = CONFIG_CONSTANTS_OFFSET;

	u_char *char_ptr;

	/* fail if no nand detected */
	if (nand_info[0].type == 0) {
        printf("ERROR: %s -> No NAND detected...\n", __FUNCTION__);
		return 1;
    }

	blocksize = nand_info[0].erasesize;
	if (!blocksize) {
        printf("ERROR: %s -> NAND erase size is zero...\n", __FUNCTION__);
		return 1;
    }
	len = min(blocksize, CONFIG_CONSTANTS_SIZE);

    printf("INFO: %s -> requested offset is 0x%08x\n", __FUNCTION__, offset);
    if (CONFIG_CONSTANTS_OFFSET != offset) {
        // redundant read so need to find primary offset
        while ((nand_block_isbad(&nand_info[0], primary_offset)) &&
               (primary_offset < end)) {
            printf("INFO: %s -> offset 0x%08x is bad\n",
                   __FUNCTION__, primary_offset);
            primary_offset += blocksize;
        }
        // redundant offset is adjusted based on the
        // valid primary offset
        offset = primary_offset + (offset - CONFIG_CONSTANTS_OFFSET);
        printf("INFO: %s -> redundant offset is 0x%08x\n", __FUNCTION__, offset);
    }

	while (amount_loaded < CONFIG_CONSTANTS_SIZE && offset < end) {
		if (nand_block_isbad(&nand_info[0], offset)) {
            printf("INFO: %s -> offset 0x%08x is bad\n",
                   __FUNCTION__, offset);
			offset += blocksize;
		} else {
			char_ptr = &buf[amount_loaded];
			if (nand_read(&nand_info[0], offset, &len, char_ptr)) {
                printf("ERROR: %s -> NAND read failed for offset 0x%08x...\n",
                       __FUNCTION__, offset);
				return 1;
            } else {
                printf("INFO: %s -> 0x%08x bytes read from offset 0x%08x\n",
                       __FUNCTION__, len, offset);
            }
			offset += blocksize;
			amount_loaded += len;
		}
	}
	if (amount_loaded != CONFIG_CONSTANTS_SIZE) {
        printf("ERROR: %s -> Amount loaded(0x%08x) != constants size(0x%08x...\n",
               __FUNCTION__, amount_loaded, CONFIG_CONSTANTS_SIZE);
		return 1;
    }

	return 0;
}



int do_const(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	char*   cmd = NULL;
	int     i;
	int     rcode = 0;

	/* at least two arguments please */
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	/*
	 * Syntax is:
	 *   0     1   2    3
	 *   const set name [value]
	 */
	if (strcmp(cmd, "set") == 0) {
        if (argc < 3)
		    goto usage;
        return _do_const_set(flag, argc, argv);
	}

	/*
	 * Syntax is:
	 *   0     1     2
	 *   const print [name]
	 */
	if (strcmp(cmd, "print") == 0) {
        if (argc == 2) {
            /* print all constants vars */
            rcode = const_print(NULL);
            if (!rcode)
                return 1;
            printf("\nConstants size: %d/%ld bytes\n",
                    rcode, (ulong)CONFIG_CONSTANTS_SIZE);
            return 0;
        }

        /* print selected constants vars */
        for (i = 2; i < argc; ++i) {
            int rc = const_print(argv[i]);
            if (!rc) {
                printf("## Error: \"%s\" not defined in Constants\n", argv[i]);
                ++rcode;
            }
        }

        return rcode;
	}

	/*
	 * Syntax is:
	 *   0     1
	 *   const reload
	 */
	if (strcmp(cmd, "reload") == 0) {
		if (gd->const_valid == 0)
			return 1;

		const_relocate_spec();
		return 0;
	}

	/*
	 * Syntax is:
	 *   0     1
	 *   const save
	 */
	if (strcmp(cmd, "save") == 0) {
		return saveconst();
	}

usage:
	return cmd_usage(cmdtp);
}

U_BOOT_CMD(
	const, CONFIG_SYS_MAXARGS, 1, do_const,
	"Permanent constants - preserved during fw update process",
	"set name value - set constants variable 'name' to 'value ...'\n"
	"const set name       - delete constants variable 'name'\n"
	"const print          - print values of all constants variables\n"
	"const print name     - print value of constants variable 'name'\n"
	"const reload         - reloads constants variables from FLASH\n"
	"const save           - save constants variables to persistent storage"
);

