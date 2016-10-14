/*
 * This file contains the common U-Boot script for the firmware upgrade process.
 *
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __SUE_FWUPDATE_BCM_H
#define __SUE_FWUPDATE_BCM_H

/*
 *
 * METHODS:
 *		readDtb                         - loads DTB file for board from active dt-blob partition,
 *		readUimage                      - loads uImage to RAM
 *		nand_boot                       - boot active image
 *		ab_boot                         - main method
 *
 * VARIABLES:
 * 		ACTIVE_IMAGE                    - active image. A or B.
 * 		boot_count                      - actual count of uncorrect reboots for active image
 * 		BOOT_COUNT_A                    - actual count of uncorrect reboots for A image
 * 		BOOT_COUNT_B                    - actual count of uncorrect reboots for B image
 * 		MAX_BOOT_COUNT                  - limit of uncorrcet reboots
 * 		ab_load_addr                    - base RAM address for u-boot operations
 * 		dts                             - active dt-blob partition
 * 		kernel                          - active kernel partition
 * 		nandroot                        - active nandroot parameter.
 * 		board_name                      - name of the board, used for locating DTB file in cramfs image
 * 		board_rev                       - rev of the board, used for locating DTB file in cramfs image
 */

 /* TODO: generalize board_name, UBI_VOL_PREFIX, for one prefix and use it for fdt, ubi, etc... */

#define SUE_FWUPDATE_EXTRA_ENV_SETTINGS \
	"loadaddr=0x63200000\0" \
	"fdtaddr=0x63F80000\0" \
	"rdaddr=0x81000000\0" \
	"bootfile=zImage\0" \
	"board_name=bcm958305k\0" \
	"fdtfile=\0" \
	"console=ttyS0,115200n8\0" \
	"optargs=''\0" \
	"ab_load_addr=0x63000000\0" \
	"dtb_load_addr=0x64000000\0" \
	"wdtargs=''\0" \
	"UBI_VOL_PREFIX=bcm958305k\0" \
	"bootargs_defaults=setenv bootargs " \
		"console=${console} " \
		"${wdtargs} " \
		"optargs=${optargs}\0" \
	"eraseSettings=" \
		"if nand erase.part settings; " \
		"then " \
			"echo \"INFO: nand erase successful\"; " \
		"else " \
			"echo \"ERROR: nand erase failed\"; " \
		"fi; \0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"readuImage=" \
		"if nand read ${ab_load_addr} ${kernel}; " \
		"then " \
			"echo \"INFO: kernel partition load successful\"; " \
		"else " \
			"echo \"ERROR: cannot load kernel image from nand\"; " \
			"reset; " \
		"fi;\0" \
\
	"readDtb=" \
		"if nand read ${dtb_load_addr} ${dts}; " \
		"then " \
			"echo \"INFO: dts file load successful\"; " \
			"if fdt addr ${dtb_load_addr} ; " \
			"then " \
				"echo \"INFO: fdt addr set successful\"; " \
				"if fdt resize; " \
				"then " \
					"echo \"INFO: fdt resize successful\"; " \
				"else " \
					"echo \"ERROR: cannot resize fdt\"; " \
					"reset; " \
				"fi; " \
			"else " \
				"echo \"ERROR: cannot set fdt addr\"; " \
				"reset; " \
			"fi; " \
		"else " \
			"echo \"ERROR: cannot load DTS image from nand\"; " \
			"reset; " \
		"fi;\0" \
\
	"nandrootfstype=ubifs rootwait=1\0" \
	"nandargs=setenv bootargs console=${console} " \
		"root=${nandroot} noinitrd ${wdtargs} ${optargs} " \
		"rootfstype=${nandrootfstype} ${mtdparts}\0" \
	"nand_boot=echo \"Booting from nand ...\"; " \
		"run nandargs; " \
		"echo \"INFO: loading kernel image into RAM...\"; " \
		"bstate booting; " \
		"run readDtb; " \
		"run readuImage; " \
		"echo \"INFO: booting kernel image...\"; " \
		"bootz ${ab_load_addr} - ${dtb_load_addr} ${bootargs}; " \
		"echo \"INFO: kernel boot failed...\"; " \
		"echo \"INFO: resetting...\"; " \
		"reset;\0" \
	"panicargs=panic=10 mem=512M\0" \
\
	"update_boot_count=" \
		"if test ${ACTIVE_IMAGE} = A; " \
		"then " \
			"setenv BOOT_COUNT_A ${boot_count}; " \
		"else " \
			"setenv BOOT_COUNT_B ${boot_count}; " \
		"fi;\0" \
\
	"change_active_image=" \
		"if test ${ACTIVE_IMAGE} = A; " \
		"then " \
			"setenv ACTIVE_IMAGE B; " \
		"else " \
			"setenv ACTIVE_IMAGE A; " \
		"fi;\0" \
\
	"BOOT_COUNT_A=0\0" \
	"BOOT_COUNT_B=0\0" \
	"MAX_BOOT_COUNT=3\0" \
	"ACTIVE_IMAGE=A\0" \
	"dts=dt-blob-a\0" \
	"kernel=kernel-a\0" \
	"nandroot=ubi0:bcm958305k-rootfs ubi.mtd=8,2048 rw \0 " \
\
	"ab_boot=" \
		"while test ${BOOT_COUNT_A} < ${MAX_BOOT_COUNT} || test ${BOOT_COUNT_B} < ${MAX_BOOT_COUNT}; " \
		"do " \
			"echo \"INFO: attempting boot...\"; " \
			"if test ${ACTIVE_IMAGE} = A; " \
			"then " \
				"setexpr boot_count ${BOOT_COUNT_A}; " \
				"setenv dts dt-blob-a; " \
				"setenv kernel kernel-a; " \
				"setenv nandroot ubi0:bcm958305k-rootfs ubi.mtd=8,2048 rw; " \
			"else " \
				"setexpr boot_count ${BOOT_COUNT_B}; " \
				"setenv dts dt-blob-b; " \
				"setenv kernel kernel-b; " \
				"setenv nandroot ubi0:bcm958305k-rootfs ubi.mtd=11,2048 rw; " \
			"fi; " \
			"echo \"INFO: active image ${ACTIVE_IMAGE}\"; " \
			"echo \"INFO: boot_count = ${boot_count}\"; " \
			"if test ${boot_count} < ${MAX_BOOT_COUNT}; " \
			"then " \
				"setexpr boot_count ${boot_count} + 1; " \
				"run update_boot_count; " \
				"saveenv; " \
				"bstate normal; " \
				"run nand_boot; " \
			"else " \
				"echo \"INFO: max boot count reached. Switch to other image\"; " \
				"run change_active_image;"\
				"saveenv; " \
			"fi; " \
		"done; " \
		"run tftp_auto_update\0" \
\
	"tftp_auto_update=echo \"Placeholder for automatic tftp update.\" ;\0"

#define SUE_FWUPDATE_BCM_BOOTCOMMAND \
	"echo \"INFO: attempting A/B boot...\"; " \
	"run ab_boot;" \

#define SUE_FWUPDATE_BCM_ALTBOOTCOMMAND \
	"echo \"ERROR: Maximum boot count reached!\"; "

#endif /* __SUE_FWUPDATE_BCM_H */
