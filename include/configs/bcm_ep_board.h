/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BCM_EP_BOARD_H
#define __BCM_EP_BOARD_H

#include <asm/arch/configs.h>
#include <configs/sue_fwupdate_bcm.h>

/* Architecture, CPU, chip, etc */
#define CONFIG_ARMV7

#define CONFIG_SYS_GENERIC_BOARD

#define CONFIG_ARM_GIC_BASE_ADDRESS 0x19020000

/*
 * Memory configuration
 * (these must be defined elsewhere)
 */
#ifndef CONFIG_SYS_TEXT_BASE
#error	CONFIG_SYS_TEXT_BASE must be defined!
#endif
#ifndef CONFIG_SYS_SDRAM_BASE
#error	CONFIG_SYS_SDRAM_BASE must be defined!
#endif
#ifndef CONFIG_SYS_SDRAM_SIZE
#error	CONFIG_SYS_SDRAM_SIZE must be defined!
#endif

#define CONFIG_NR_DRAM_BANKS		1

#define CONFIG_SYS_MALLOC_LEN		(4 * 1024 * 1024)
#define CONFIG_STACKSIZE		(512 * 1024)

/* Some commands use this as the default load address */
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE

/* No mtest functions as recommended */
#undef CONFIG_CMD_MEMORY

#define CONFIG_BCM_RAMDUMP

/*
 * This is the initial SP which is used only briefly for relocating the u-boot
 * image to the top of SDRAM. After relocation u-boot moves the stack to the
 * proper place.
 */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Serial Info */
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL

#define CONFIG_BAUDRATE			115200

#define CONFIG_SYS_NO_FLASH	/* Not using NAND/NOR unmanaged flash */

/* console configuration */
#define CONFIG_SYS_CBSIZE		(10 * 1024)	/* Console buffer size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
			sizeof(CONFIG_SYS_PROMPT) + 16)	/* Printbuffer size */
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

/*
 * One partition type must be defined for part.c
 * This is necessary for the fatls command to work on an SD card
 * for example.
 */
#define CONFIG_DOS_PARTITION

/* eMMC configuration */

#define CFG_FASTBOOT_MMC_NO 0
#define CONFIG_CMD_GPT
#define CONFIG_PARTITION_UUIDS
#ifdef CONFIG_GENERIC_MMC
#define CONFIG_CMD_EMMC
#endif
#define CONFIG_SYS_MMC_ENV_DEV 0
/* Ethernet */
#define CONFIG_CMD_MII
#define CONFIG_PHYLIB
#define CONFIG_PHY_BROADCOM
#define CONFIG_PHY_RESET_DELAY 10000 /* PHY reset delay in us*/

/* Allows faster TFTP and NFS download */
#define CONFIG_IP_DEFRAG
#define CONFIG_TFTP_BLOCKSIZE  8192
#define CONFIG_NFS_READ_SIZE   8192

/* version string, parser, etc */
#define CONFIG_VERSION_VARIABLE
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_CMDLINE_EDITING
#define CONFIG_COMMAND_HISTORY
#define CONFIG_SYS_LONGHELP
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_CRC32_VERIFY
#define CONFIG_MX_CYCLIC

/* Commands */
#include <config_cmd_default.h>

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ELF
#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_CMD_I2S

#define CONFIG_EFI_PARTITION

/* Enable devicetree support */
#define CONFIG_OF_LIBFDT

/* SHA hashing */
#define CONFIG_CMD_HASH
#define CONFIG_HASH_VERIFY
#define CONFIG_SHA1
#define CONFIG_SHA256

/* Enable Time Command */
#define CONFIG_CMD_TIME

#define CONFIG_CMD_BOOTZ

/* Misc utility code */
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CRC32_VERIFY

#define DISABLE_DT_INITRD_RELOC \
	"fdt_high=ffffffff\0"\
	"initrd_high=ffffffff\0"
#define CONFIG_SUPPORT_RAW_INITRD
#define DEFAULT_ENV_SETTINGS \
	DISABLE_DT_INITRD_RELOC

#ifndef ARCH_ENV_SETTINGS
#define ARCH_ENV_SETTINGS
#endif

/*#define MTDIDS_DEFAULT		"nand0=brcmnand.0"*/
#define MTDPARTS_DEFAULT	"mtdparts=brcmnand.0:1m(boot1),1m(m0patch),1m(gpt),2m(ssb),2m(u-boot),2m(u-boot-env),1m(constants),128m(settings),16m(kernel),2m(dts),434m(rootfs),434m(download)"
#define CONFIG_BOOTCOMMAND	SUE_FWUPDATE_BCM_BOOTCOMMAND
#define CONFIG_ALTBOOTCOMMAND	SUE_FWUPDATE_BCM_ALTBOOTCOMMAND

#define CONFIG_BOOTCOUNT_LIMIT	6

#define CONFIG_EXTRA_ENV_SETTINGS \
/*	ARCH_ENV_SETTINGS*/ \
	SUE_FWUPDATE_EXTRA_ENV_SETTINGS \
	DEFAULT_ENV_SETTINGS

#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_NAND_TRIMFFS
#define CONFIG_CMD_MKFS_UBIFS

#if (defined CONFIG_MTD_UBI_BEB_RESERVE)
#undef CONFIG_MTD_UBI_BEB_RESERVE
#define CONFIG_MTD_UBI_BEB_RESERVE  (2)
#endif

#define CONFIG_CMD_CRAMFS
#define CONFIG_CRAMFS_CMDLINE

/* update  support */
#define CONFIG_CMD_SFU_PARSER
#define CONFIG_SFU_RAM
#undef SFU_DEBUG
#undef CRC32_DEBUG
#undef MD5_DEBUG
#undef SHA256_DEBUG

#define CONFIG_CMD_SETEXPR


#endif /* __BCM_EP_BOARD_H */
