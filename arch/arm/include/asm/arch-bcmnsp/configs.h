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
#define CONFIG_NS_PLUS
#define CONFIG_SYS_SDRAM_BASE   0x61000000

/* Serial Info */
/* no padding */
#define CONFIG_SYS_NS16550_REG_SIZE	1

#define CONFIG_SYS_NS16550_CLK		0x03b9aca0
#define CONFIG_CONS_INDEX		1
#define CONFIG_SYS_NS16550_COM1		0x18000300

/* I2C default speed */
#define CONFIG_SYS_I2C_SPEED	0

/* general commands for updates/booting */
#define CONFIG_CMD_MATH
#define CONFIG_CMD_MALLOC
#define CONFIG_CMD_STRCMP
#define CONFIG_SYS_HUSH_PARSER

/* Ethernet */
#define CONFIG_BCMIPROC_ETH
#define IPROC_ETH_MALLOC_BASE	0xD00000
#define CONFIG_GMAC_NUM			1
/* #define CONFIG_NET_MULTI */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CMD_PING

/* NAND configuration */
#define CONFIG_IPROC_NAND
#define CONFIG_SYS_NAND_BASE		0
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_MAX_NAND_CHIPS	1
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_CMD_NAND

/* Configuration for Micron MT29F8G08ABACA */
#define CONFIG_NAND_CHIPSIZE	(1 << 30)	/* 1GB */
#define CONFIG_NAND_BLOCKSIZE	(1 << 18)	/* 256KB */
#define CONFIG_NAND_PAGE_SHIFT	12		/* 4KiB */
#define CONFIG_NAND_BLOCK_SHIFT	18		/* 256 KiB */

#define MTD_PART_BOOT1_SIZE	0x200000
#define MTD_PART_GPT_SIZE	0x100000
#define MTD_PART_SSB_SIZE	0x200000
#define MTD_PART_UBOOT_SIZE	0x200000
#define MTD_PART_UBOOT_ENV_SIZE	0x200000
#define MTD_PART_DTB_SIZE	0x100000
#define MTD_PART_KERNEL_SIZE	0x1000000
#define MTD_PART_ROOT_SIZE		0xc000000

/*
 * WARNING: When changing the partitions order make sure you update
 *          CONFIG_ENV_OFFSET accordingly.
 */
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE		0x2000
#define CONFIG_ENV_RANGE	(2 * CONFIG_NAND_BLOCKSIZE)
#define CONFIG_ENV_OFFSET	(MTD_PART_BOOT1_SIZE + MTD_PART_GPT_SIZE + \
				 MTD_PART_SSB_SIZE + MTD_PART_UBOOT_SIZE)
#define CONFIG_ENV_OFFSET_REDUND (CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE)
/*
 * Check that the u-boot-env partition is big enough to accomodate the
 * u-boot environment data and its redundant copy
 */
#if ((2 * CONFIG_ENV_RANGE) > MTD_PART_UBOOT_ENV_SIZE)
#error u-boot-env partition size is to small
#endif

/* MTD configuration */
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_MTDPARTS_SPREAD

#define MTDIDS_DEFAULT		"nand0=iproc_nand"

#define MTD_PART_BOOT1		"mtdparts add nand0 2m boot1;"
#define MTD_PART_GPT		"mtdparts add nand0 1m gpt;"
#define MTD_PART_SSB		"mtdparts add nand0 2m ssb;"
#define MTD_PART_UBOOT		"mtdparts add nand0 2m u-boot;"
#define MTD_PART_UBOOT_ENV	"mtdparts add nand0 2m u-boot-env;"
#define MTD_PART_DTBLOB		"mtdparts add nand0 1m dt-blob;"
#define MTD_PART_KERNEL		"mtdparts add nand0 16m kernel;"
#define MTD_PART_ROOT		"mtdparts add nand0 192m root;"
#define MTD_PART_LIST \
	MTD_PART_BOOT1 \
	MTD_PART_GPT \
	MTD_PART_SSB \
	MTD_PART_UBOOT \
	MTD_PART_UBOOT_ENV \
	MTD_PART_DTBLOB \
	MTD_PART_KERNEL \
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
	"loadaddr=62000000\0"

#define TFTP_PREFIX \
	"tftp_prefix=nsp\0"

#define TFTP_UPDATE \
"tftp_update="\
"malloc tmp 2000;"\
"tftp ${tmp} ${tftp_update_prefix}.tftp-update;"\
"source ${tmp};"\
"free tmp\0"

#define KERNEL_LOADADDR_CFG \
	"zimage_loadaddr=62000000\0" \
	"dtb_loadaddr=62800000\0"

#define INITRD_LOADADDR_CFG \
	KERNEL_LOADADDR_CFG  \
	"initrd_loadaddr=62900000\0"

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

#define CONSOLE_ARGS "console_args=console=ttyS0,115200n8\0"
#define MEM_ARGS "mem_args=mem=128m\0"

#define SETBOOTARGS "setbootargs=setenv bootargs ${console_args} ${mem_args}\0"

#define NFS_DEVICE "ethif=eth2\0"

#define IP_CFG_BOOTARGS	"ip=${ipaddr}::${gatewayip}:${netmask}::${ethif}:off"

#define NFS_GET_DTBLOB \
	"nfs_get_dtblob=nfs ${dtb_loadaddr} ${nfs_root}/flash/bin.dt-blob;"\
	"fdt addr ${dtb_loadaddr}\0"

#define NFS_BOOTARGS "nfs_bootargs=root=/dev/nfs rw nfsroot=${serverip}:${nfs_root},tcp rootwait " IP_CFG_BOOTARGS "\0"

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

#define MTD_BOOTARGS "mtd_bootargs=${mtdparts} ubi.mtd=${rootpartidx} root=ubi0:rootfs rootfstype=ubifs rw " IP_CFG_BOOTARGS "\0"

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
	TFTP_PREFIX \
	TFTP_UPDATE \
	INITRD_LOADADDR_CFG \
	CONSOLE_ARGS \
	MEM_ARGS \
	SETBOOTARGS \
	INITRD_BOOTCMD \
	NFS_DEVICE \
	NFS_GET_DTBLOB \
	NFS_BOOTARGS \
	NFS_BOOTCMD \
	NFS_UPDATE

#define CONFIG_ZERO_BOOTDELAY_CHECK /* check keypress even if bootdelay==0 */
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTCOMMAND "run initrd_bootcmd"

#endif /* __ARCH_CONFIGS_H */
