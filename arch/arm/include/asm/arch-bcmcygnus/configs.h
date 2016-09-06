/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ARCH_CONFIGS_H
#define __ARCH_CONFIGS_H

#include <linux/stringify.h>
#include <asm/iproc-common/configs.h>

/* uArchitecture specifics */
#define CONFIG_CYGNUS
#define CONFIG_SYS_SDRAM_BASE   0x60000000

/* Size of Reserved Secure DRAM area*/
#ifndef CONFIG_RESERVED_SEC_MEM
#define CONFIG_RESERVED_SEC_MEM 0x00400000
#endif

#define CONFIG_MISC_INIT_R

/* Legacy format support is required for scripts that uses legacy format */
#define CONFIG_IMAGE_FORMAT_LEGACY 1
/* Flat Image Tree support. */
#define CONFIG_FIT		1
#define CONFIG_FIT_VERBOSE	1
/* Enable FDT */
#define CONFIG_OF_CONTROL	1
/* Build FDT separately and append to binary */
#define CONFIG_OF_SEPARATE	1
/* Enable verified boot */
#define CONFIG_FIT_SIGNATURE	1
#define CONFIG_RSA		1

/* Enable reset L3 command */
#define CONFIG_CMD_RESET	1

/* Enable emmc boot partition support */
#define CONFIG_SUPPORT_EMMC_BOOT

/* Set arm clock speed: 1000 = 1GHz */
#define CONFIG_ARMCLK 1000

/* Serial Info */
/* Post pad 3 bytes after each reg addr */
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_MEM32

#define CONFIG_SYS_NS16550_CLK		100000000
#define CONFIG_SYS_NS16550_CLK_DIV	54
#define CONFIG_SERIAL_MULTI
#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		0x18023000

/* I2C default speed */
#define CONFIG_SYS_I2C_SPEED	100000 /* 100 kHz */

/* general commands for updates/booting */
#define CONFIG_CMD_MATH
#define CONFIG_CMD_MALLOC
#define CONFIG_CMD_STRCMP
#define CONFIG_SYS_HUSH_PARSER

#ifdef CONFIG_LCD
#define CONFIG_PL111_LCD
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_RAW_BMP_LOGO
#define CONFIG_SYS_WHITE_ON_BLACK
#define CONFIG_CMD_BMP
#define CONFIG_VIDEO_BMP_GZIP
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE 800*480*4
#endif

/* Ethernet */
#define CONFIG_BCM_SF2_ETH
#define CONFIG_BCM_SF2_ETH_GMAC
#define CONFIG_BCM_ROBO
/* #define CONFIG_NET_MULTI */
#define CONFIG_ENV_OVERWRITE

/* IPROC SPI API required if any of PL022 or QSPI is enabled */
#define CONFIG_IPROC_SPI

/* SPI Flash */
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_SST
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_SPI_FLASH_ATMEL
#define CONFIG_SPI_FLASH_EON
#define CONFIG_SPI_FLASH_GIGADEVICE
#define CONFIG_IPROC_QSPI
#define CONFIG_IPROC_QSPI_BUS			0
#define CONFIG_IPROC_QSPI_CS			0
#define CONFIG_IPROC_QSPI_BASE_REG		0x18047000
#define CONFIG_IPROC_QSPI_CRU_CONTROL_REG	0x1800e000
#define CONFIG_IPROC_BSPI_DATA_LANES            1
#define CONFIG_IPROC_BSPI_ADDR_LANES            1
#define CONFIG_IPROC_BSPI_READ_CMD              0x0b
#define CONFIG_IPROC_BSPI_READ_DUMMY_CYCLES     8
#define CONFIG_SF_DEFAULT_SPEED                 62500000
#define CONFIG_SF_DEFAULT_MODE                  SPI_MODE_3

/* PL022 SPI */
#define CONFIG_IPROC_PL022_SPI
#define CONFIG_IPROC_PL022_SPI_BUS              1
/* SPI clock 125MHz */
#define CONFIG_IPROC_SPI_CLK                    125000000

/* MMC */
#define CONFIG_MMC
#define CONFIG_MMC_SDMA
#undef CONFIG_MMC_TRACE
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_MMC
#define CONFIG_SDHCI
#define CONFIG_IPROC_SDHCI
#define CONFIG_MMC_SDHCI_IO_ACCESSORS
#undef CONFIG_MMC_SDHCI_IO_ACCESSORS_TRACE
#define CONFIG_SYS_SDIO0_BASE			0x18041000
#define CONFIG_SYS_SDIO1_BASE			0x18043000
#define CONFIG_SYS_SDIO_MIN_CLK			400000
#define CONFIG_SYS_SDIO_MAX_CLK			200000000

/* NAND configuration */
#define CONFIG_IPROC_NAND
#define CONFIG_SYS_NAND_BASE		0
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_MAX_NAND_CHIPS	1
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_CMD_NAND

#if defined(CONFIG_BCM_NAND_SS_K9F4G08U0E)
/* Configuration for Samsung K9F4G08U0E */
#define CONFIG_IPROC_NAND_TIMING_MODE	4	/* for 25ns R/W cycles */
#define CONFIG_NAND_CHIPSIZE	(1 << 29)	/* 512MB */
#define CONFIG_NAND_BLOCKSIZE	(1 << 17)	/* 128KB */
#define CONFIG_NAND_PAGE_SHIFT	11			/* 2KiB */
#define CONFIG_NAND_BLOCK_SHIFT	17			/* 128 KiB */
#elif defined(CONFIG_BCM_NAND_TC58NVG2S0HBAI6)
/* Toshiba TC58NVG2S0HBA16 */
#define CONFIG_IPROC_NAND_TIMING_MODE	4	/* for 25ns R/W cycles */
#define CONFIG_NAND_CHIPSIZE	(1 << 29)	/* 512MB */
#define CONFIG_NAND_BLOCKSIZE	(1 << 18)	/* 256KB */
#define CONFIG_NAND_PAGE_SHIFT	12			/* 4KiB */
#define CONFIG_NAND_BLOCK_SHIFT	18			/* 256 KiB */
#elif defined(CONFIG_BCM_NAND_TC58NVG0S3HBAI6)
/* Toshiba TC58NVG0S3HBAI6 */
#define CONFIG_IPROC_NAND_TIMING_MODE	4	/* for 25ns R/W cycles */
#define CONFIG_NAND_CHIPSIZE	(1 << 27)	/* 128MB */
#define CONFIG_NAND_BLOCKSIZE	(1 << 17)	/* 128KB */
#define CONFIG_NAND_PAGE_SHIFT	11			/* 2KiB */
#define CONFIG_NAND_BLOCK_SHIFT	17			/* 128 KiB */
#elif defined(CONFIG_BCM_NAND_TC58NVG1S3HBAI6)
/* Toshiba TC58NVG0S3HBAI6 */
#define CONFIG_IPROC_NAND_TIMING_MODE	4	/* for 25ns R/W cycles */
#define CONFIG_NAND_CHIPSIZE	(1 << 28)	/* 256MB */
#define CONFIG_NAND_BLOCKSIZE	(1 << 17)	/* 128KB */
#define CONFIG_NAND_PAGE_SHIFT	11			/* 2KiB */
#define CONFIG_NAND_BLOCK_SHIFT	17			/* 128 KiB */
#else
/* Configuration for Micron MT29F8G08ABACA */
#define CONFIG_IPROC_NAND_TIMING_MODE	5
#define CONFIG_NAND_CHIPSIZE	(1 << 30)	/* 1GB */
#define CONFIG_NAND_BLOCKSIZE	(1 << 18)	/* 256KB */
#define CONFIG_NAND_PAGE_SHIFT	12		/* 4KiB */
#define CONFIG_NAND_BLOCK_SHIFT	18		/* 256 KiB */
#endif

#define MTD_PART_BOOT1_SIZE	0x200000
#define MTD_PART_GPT_SIZE	0x100000
#define MTD_PART_SSB_SIZE	0x200000
#define MTD_PART_UBOOT_SIZE	0x200000
#define MTD_PART_UBOOT_ENV_SIZE	0x200000
#define MTD_PART_DTB_SIZE	0x100000
#define MTD_PART_KERNEL_SIZE	0x1000000

/* reserve 4MB of NAND for secure storage */
#ifdef CONFIG_RESERVE_SECSTOR_PARTITION
#define MTD_PART_KEYSTORE_SIZE 0x400000
#if defined(CONFIG_BCM_NAND_TC58NVG1S3HBAI6)
#define MTD_PART_ROOT_SIZE	0xc400000
#else
#define MTD_PART_ROOT_SIZE	0x1bc00000
#endif
#else
#if defined(CONFIG_BCM_NAND_TC58NVG1S3HBAI6)
#define MTD_PART_ROOT_SIZE	0xc800000
#else
#define MTD_PART_ROOT_SIZE	0x1c000000
#endif
#endif
/*
 * WARNING: When changing the partitions order make sure you update
 *          CONFIG_ENV_OFFSET accordingly.
 */
#define CONFIG_ENV_SIZE		(128 * 1024)
#ifdef CONFIG_ENV_IS_IN_MMC
#define UBOOT_ENV_PART_SIZE	0x200000
#define CONFIG_ENV_OFFSET	0x00700000 /* Must match mmc gpt table layout */
#define CONFIG_ENV_RANGE	(2 * CONFIG_ENV_SIZE)
#else /* !CONFIG_ENV_IS_IN_MMC */
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET	(MTD_PART_BOOT1_SIZE + MTD_PART_GPT_SIZE + \
				 MTD_PART_SSB_SIZE + MTD_PART_UBOOT_SIZE)
#define CONFIG_ENV_RANGE	(2 * CONFIG_NAND_BLOCKSIZE)
#define UBOOT_ENV_PART_SIZE	MTD_PART_UBOOT_ENV_SIZE
#endif /* CONFIG_ENV_IS_IN_MMC */
/*#define CONFIG_ENV_OFFSET_REDUND (CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE)*/

/*
 * Check that the u-boot-env partition is big enough to accomodate the
 * u-boot environment data and its redundant copy
 */
#if ((2 * CONFIG_ENV_RANGE) > UBOOT_ENV_PART_SIZE)
#error u-boot-env partition size is to small
#endif

/* MTD configuration */
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_MTDPARTS_SPREAD

#define MTDIDS_DEFAULT		"nand0=brcmnand.0"

#define MTD_PART_BOOT1		"mtdparts add nand0 1m boot1;"
#define MTD_PART_M0PATCH	"mtdparts add nand0 1m m0patch;"
#define MTD_PART_GPT		"mtdparts add nand0 1m gpt;"
#define MTD_PART_SSB		"mtdparts add nand0 2m ssb;"
#define MTD_PART_UBOOT		"mtdparts add nand0 2m u-boot;"
#define MTD_PART_UBOOT_ENV	"mtdparts add nand0 2m u-boot-env;"
#define MTD_PART_DTBLOB		"mtdparts add nand0 1m dt-blob;"
#define MTD_PART_KERNEL		"mtdparts add nand0 16m kernel;"

#ifdef CONFIG_RESERVE_SECSTOR_PARTITION
#define MTD_PART_KEYSTORE	"mtdparts add nand0 4m keystore;"
#if defined(CONFIG_BCM_NAND_TC58NVG1S3HBAI6)
#define MTD_PART_ROOT		"mtdparts add nand0 196m root;"
#else
#define MTD_PART_ROOT		"mtdparts add nand0 444m root;"
#endif
#else
#define MTD_PART_KEYSTORE
#if defined(CONFIG_BCM_NAND_TC58NVG1S3HBAI6)
#define MTD_PART_ROOT		"mtdparts add nand0 200m root;"
#else
#define MTD_PART_ROOT		"mtdparts add nand0 448m root;"
#endif
#endif

#define MTD_PART_LIST \
	MTD_PART_BOOT1 \
	MTD_PART_M0PATCH \
	MTD_PART_GPT \
	MTD_PART_SSB \
	MTD_PART_UBOOT \
	MTD_PART_UBOOT_ENV \
	MTD_PART_DTBLOB \
	MTD_PART_KERNEL \
	MTD_PART_KEYSTORE \
	MTD_PART_ROOT

/* Export page shift and block shift for external use */
#define MTD_PAGE_SHIFT_BASE10 "mtd_page_shift_base10=" __stringify(CONFIG_NAND_PAGE_SHIFT) "\0"
#define MTD_BLOCK_SHIFT_BASE10 "mtd_block_shift_base10=" __stringify(CONFIG_NAND_BLOCK_SHIFT) "\0"

/* Setup the mtd partitions. Extend this by adding kernel, bootloaders, etc. */
#define MTD_MTDPARTS \
	"mtd_mtdparts=setenv mtdids " MTDIDS_DEFAULT ";" \
	"mtdparts delall;" MTD_PART_LIST "; "\
	"saveenv\0"

#define BASE_INFO \
	"loadaddr=63000000\0" \
	"splashimage=61000000\0"

#define TFTP_PREFIX \
	"tftp_prefix=cygnus\0"

#define TFTP_UPDATE \
"tftp_update="\
"malloc tmp 2000;"\
"tftp ${tmp} ${tftp_update_prefix}.tftp-update;"\
"source ${tmp};"\
"free tmp\0"

#define ANDROID_TFTP_UPDATE \
	"android_tftp_update=" \
	"setenv mtdparts mtdparts=brcmnand.0:1m(boot1),1m(m0patch),1m(gpt),"\
	"2m(ssb),2m(u-boot),2m(u-boot-env),1m(dt-blob),16m(kernel),"\
	"290m(system),290m(data),228m(cache);" \
	"setenv mtd_mtdparts;" \
	"run tftp_update\0"

#define KERNEL_LOADADDR_CFG \
	"zimage_loadaddr=63000000\0" \
	"dtb_loadaddr=63800000\0"

#define INITRD_LOADADDR_CFG \
	KERNEL_LOADADDR_CFG  \
	"initrd_loadaddr=63900000\0"

#define LOAD_KERNEL \
	"tftp ${zimage_loadaddr} ${tftp_prefix}.zimage;"\
	"tftp ${dtb_loadaddr} ${tftp_prefix}.dtb;"\
	"run setbootargs;"\
	"fdt addr ${dtb_loadaddr};"\
	"fdt chosen;"

#define INITRD_BOOTCMD \
	"initrd_bootcmd="\
	LOAD_KERNEL\
	"tftp ${initrd_loadaddr} ${tftp_prefix}.initrd;"\
	"bootz ${zimage_loadaddr} ${initrd_loadaddr} ${dtb_loadaddr}\0"

#define CONSOLE_ARGS "console_args=console=ttyS0,115200n8 vt.global_cursor_default=0\0"
#define EXTRA_ARGS "extra_args=earlyprintk\0"

#ifdef CONFIG_BCM_ETH_LANESWAP
#define NET_ARGS "ethaddr=${ethaddr} lswap=${ethlaneswap}" \
	" ip=${ipaddr}::${gatewayip}:${netmask}::${ethif}:off"
#else
#define NET_ARGS "ethaddr=${ethaddr} " \
	" ip=${ipaddr}::${gatewayip}:${netmask}::${ethif}:off"
#endif

#define BASE_ARGS "${console_args} ${extra_args} ${reset_args}"
#define SETBOOTARGS "setbootargs=setenv bootargs " BASE_ARGS " " NET_ARGS "\0"

#define NFS_DEVICE "ethif=eth0\0"

#define NFS_GET_DTBLOB \
	"nfs_get_dtblob=nfs ${dtb_loadaddr} ${nfs_root}/flash/bin.dt-blob;"\
	"fdt addr ${dtb_loadaddr}\0"

#define NFS_BOOTARGS "nfs_bootargs=root=/dev/nfs rw nfsroot=${serverip}:${nfs_root},tcp rootwait\0"

#define NFS_BOOTCMD \
	"nfs_bootcmd=" \
	"run nfs_get_dtblob;" \
	"setenv setbootargs ${setbootargs} ${nfs_bootargs};" \
	"run setbootargs;" \
	"nfs ${zimage_loadaddr} ${nfs_root}/flash/bin.kernel;" \
	"bootz ${zimage_loadaddr} - ${dtb_loadaddr}\0"

#define NFS_UPDATE \
"nfs_update="\
"malloc tmp 2000;"\
"nfs ${tmp} ${nfs_update_prefix}.nfs-update;"\
"source ${tmp};"\
"free tmp\0"

#define SD_DEVICE_NUMBER "sd_device_number=0\0"

#define SD_UPDATE \
"sd_update="\
"malloc tmp 2000;"\
"fatload mmc ${sd_device_number} ${tmp} ${sd_update_prefix}.sd-update;"\
"source ${tmp};"\
"free tmp\0"

#define GET_DTBLOB \
"get_dtblob="\
"if testenv mtdparts;"\
"then;"\
"else;"\
	"run mtd_mtdparts;"\
"fi;"\
"mtdparts setenv dt-blob;"\
"setenv maxdtsize 100000;"\
"nand read ${dtb_loadaddr} ${mtdparts_addr} ${maxdtsize};"\
"fdt addr ${dtb_loadaddr} ${maxdtsize}\0"

/* Extract the root filesystem partiton number to avoid having to hardcode /dev/mmcblk0pN */
#define MTD_SET_ROOTDEV \
"mtd_set_rootdev="\
"mtdparts setenv root;"\
"setenv rootdev /dev/mtdblock${mtdparts_entry};"\
"setenv rootpartidx ${mtdparts_entry}\0"

#define MTD_START_KNL \
"mtd_start_knl=mtdparts setenv kernel;"\
"nand read ${zimage_loadaddr} ${mtdparts_addr} 800000;"\
"bootz ${zimage_loadaddr} - ${dtb_loadaddr}\0"

#define FIT_START_KNL \
"fit_start_knl=mtdparts setenv kernel;"\
"nand read ${zimage_loadaddr} ${mtdparts_addr} 800000;"\
"bootm #conf@1\0"

#define MTD_BOOTARGS "mtd_bootargs=${mtdparts} ubi.mtd=${rootpartidx} root=ubi0:rootfs rootfstype=ubifs rw\0"

#define MTD_BOOTCMD \
"mtd_bootcmd="\
"if testenv mtdparts;"\
"then;"\
"else;"\
	"run mtd_mtdparts;"\
"fi;"\
"run get_dtblob;"\
"run mtd_set_rootdev;"\
"setenv setbootargs ${setbootargs} ${mtd_bootargs};"\
"run setbootargs;"\
"run mtd_start_knl\0"

#define FIT_BOOTCMD \
"fit_bootcmd="\
"if testenv mtdparts;"\
"then;"\
"else;"\
"run mtd_mtdparts;"\
"fi;"\
"run get_dtblob;"\
"run mtd_set_rootdev;"\
"setenv setbootargs ${setbootargs} ${mtd_bootargs};"\
"run setbootargs;"\
"run fit_start_knl\0"

#define ANDROID_NAND_BOOTCMD \
	"nand_bootcmd="\
	"mtdparts setenv kernel;"\
	"nandroid ${mtdparts_addr};"\
	"run get_dtblob;setenv setbootargs ${setbootargs} crash_ramdump=1 ${mtd_bootargs};"\
	"run setbootargs;"\
	"bootm ${zimage_loadaddr}:kernel@1 ${rd_loadaddr}:ramdisk@1 ${dtb_loadaddr}:fdt@1\0"

#define ANDROID_EMMC_BOOTCMD \
	"emmc_bootcmd="\
	"android boot;"\
	"setenv setbootargs ${setbootargs} crash_ramdump=1 ${mtd_bootargs};"\
	"run setbootargs;"\
	"bootz ${zimage_loadaddr} ${rd_loadaddr}:${rd_size} ${dtb_loadaddr}\0"

#ifdef CONFIG_BCM_ETH_LANESWAP
#define ETH_SETTINGS \
	"ethlaneswap=1\0"
#else
#define ETH_SETTINGS
#endif

#define ARCH_ENV_SETTINGS \
	BASE_INFO \
	MTD_PAGE_SHIFT_BASE10 \
	MTD_BLOCK_SHIFT_BASE10 \
	MTD_MTDPARTS \
	GET_DTBLOB \
	MTD_SET_ROOTDEV \
	MTD_START_KNL \
	MTD_BOOTARGS \
	MTD_BOOTCMD \
	ANDROID_NAND_BOOTCMD \
	ANDROID_EMMC_BOOTCMD \
	ETH_SETTINGS \
	TFTP_PREFIX \
	TFTP_UPDATE \
	ANDROID_TFTP_UPDATE \
	INITRD_LOADADDR_CFG \
	CONSOLE_ARGS \
	EXTRA_ARGS \
	SETBOOTARGS \
	INITRD_BOOTCMD \
	NFS_DEVICE \
	NFS_GET_DTBLOB \
	NFS_BOOTARGS \
	NFS_BOOTCMD \
	NFS_UPDATE \
	SD_DEVICE_NUMBER \
	SD_UPDATE \
	FIT_START_KNL \
	FIT_BOOTCMD

#define CONFIG_ZERO_BOOTDELAY_CHECK /* check keypress even if bootdelay==0 */
#define CONFIG_BOOTDELAY	1
/*#define CONFIG_BOOTCOMMAND "run initrd_bootcmd"*/

/* Diagnostics */
#include <asm/arch/configs-diags.h>

#endif /* __ARCH_CONFIGS_H */
