/*
 * This file contains the common U-Boot script for the firmware upgrade process.
 *
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __SUE_FWUPDATE_BCM_H
#define __SUE_FWUPDATE_BCM_H

/*
 * UPDATE PROCESS is fully described at: https://extern.streamunlimited.com:8443/display/Stream800/Firmware+Update+Architecture
 *
 * Update methods:
 *
 * 	USB RECOVERY:
 *
 * 		Following conditions has to be fulfilled to start USB recovery in u-boot:
 * 			board specific tests on USB recovery passes
 * 			=> this test usually check press of button connected to GPIO1_9
 * 			USB stick is plugged in during boot
 * 			USB stick contains valid file called sfupdate on first partition, which has to be fat32
 *
 * 		Following steps will be performed if conditions are fulfilled:
 * 			U-boot copy sfupdate to download partition
 * 			U-boot set FAIL flag and reset the board
 * 			Restarted U-boot loads sfupdate from download partition into RAM
 * 			U-boot verifies checksums and versions of all parts of sfupdate inside RAM
 * 			U-boot flashes boot1, u-boot and u-boot-env if version changed  In this case it also reset the board
 * 				If reset was performed than, u-boot recognize that FAIL flag is set and continues usb recovery
 * 				loads sfupdate from download partition into RAM again
 * 				verifies the checksums and versions and it will find out that boot1, u-boot and u-boot-env are up to date
 * 			FAIL flag is still set so u-boot erase the kernel
 * 			U-boot automatically flashes dts without version check.
 * 			U-boot automatically flashes rootfs without version check.
 * 			U-boot automatically flashes the kernel.
 * 			U-boot boots the kernel.
 * 			Userspace after correct boot should clear FAIL flag.
 *
 * 	NETWORK UPDATE:
 *
 * 		Steps happening in the userspace:
 * 			update image is downloaded and flashed to download partition
 * 			board is restarted with UPDATE flag set
 *
 * 		Following conditions has to be fullfilled to start net update in u-boot:
 * 			UPDATE flag is set
 *
 * 		Following steps will be performed if conditions are fulfilled:
 * 			Restarted U-boot loads sfupdate from download partition into RAM
 * 			U-boot verifies checksums and versions of all parts of sfupdate inside RAM
 * 			U-boot flashes boot1, u-boot and u-boot-env if version changed. In this case it also reset the board
 * 				If reset was performed than, u-boot recognize that UPDATE flag is set and continues with network update
 * 				loads sfupdate from download partition into RAM again
 * 				verifies the checksums and versions and it will find out that boot1, u-boot and u-boot-env are up to date
 * 			If rootfs or kernel version changed, kernel is erased
 * 			If dts version changed, dts is erased and flashed
 * 			If rootfs version changed, rootfs is erased and flashed
 * 			If rootfs or kernel version changed, kernel is flashed
 * 			U-boot clear UPDATE flag
 * 			U-boot set FAIL flag
 * 			U-boot boots the kernel
 * 			Userspace after correct boot should clear FAIL flag.
 *
 * METHODS:
 *		readDtb                         - loads DTB file for board from cramfs image, path in cramfs: /${cramfsdir}/${board_name}_${board_rev}.dtb
 *		readUimage                      - loads uImage to RAM
 *		handle_encm                     - decrypts chunk configured by last "sfu chnkhdr", if it was encrypted
 *		update_download_from_usb        - loads sfupdate file from usb and saves it into download partition
 * 		check_bootloaders_need_flashing - check boot1, u-boot, u-boot-env partition in image, set enviroments to signalize that flashing them is needed
 * 		flash_bootloaders_as_needed     - flash boot1, u-boot, u-boot-env partition as needed, decision is based on enviroment set by check_bootloaders_need_flashing
 * 		check_system_need_flashing      - check DTS, rootfs, kernel partition in image, set enviroments to signalize that flashing them is needed
 * 		flash_system_as_needed          - flash DTS, rootfs, kernel partition in image as needed, decision is based on enviroment set by check_system_need_flashing
 *		sfu_boot                        - main method
 *
 * VARIABLES:
 * 		bootcount                       - actual count of uncorrect reboots
 * 		bootlimit                       - limit of uncorrcet reboots, if reached, board does not boot anymore
 * 		sfu_load_addr                   - base RAM address for u-boot operations
 * 		boot1_vers						- version of boot1 stored in NAND
 * 		uboot_vers                      - version of u-boot stored in NAND
 * 		ubootenv_vers                   - version of u-bootenv stored in NAND
 * 		dts_vers                        - version of DTS partition stored in NAND
 * 		kernel_vers                     - version of kernel stored in NAND
 * 		rootfs_vers                     - version of rootfs stored in NAND
 * 		cramfsdir                       - directory in cramfs image where dtb file for this board should be located
 * 		board_name                      - name of the board, used for locating DTB file in cramfs image
 * 		board_rev                       - rev of the board, used for locating DTB file in cramfs image
 * 		cramfsdir                       - directory in cramfs image where dtb file for this board should be located
 * 		SFU_CHNK_ENCM                   - actual chunk encryption flag in sfupdate file, set by call sfu chnkdr command
 * 		SFU_CHNK_DATA                   - actual chunk data start address, set by call sfu chnkdr command
 * 		SFU_CHNK_SIZE                   - actual chunk data size, set by call sfu chnkdr command
 * 		SFU_CHNK_VERS                   - actual chunk version in sfupdate file, set by call sfu chnkdr command
 * 		SFU_DECRYPT_BOOT1_CHNK_SIZE       - size of boot1 chunk after decryption, set by call sfu decrypt command
 * 		SFU_DECRYPT_UBOOT_CHNK_SIZE     - size of u-boot chunk after decryption, set by call sfu decrypt command
 * 		SFU_DECRYPT_UBOOTENV_CHNK_SIZE  - size of u-boot chunk after decryption, set by call sfu decrypt command
 * 		SFU_DECRYPT_DTS_CHNK_SIZE       - size dts chunk after decryption, set by call sfu decrypt command
 * 		SFU_DECRYPT_ROOTFS_CHNK_SIZE    - size rootfs chunk after decryption, set by call sfu decrypt command
 * 		SFU_DECRYPT_KERNEL_CHNK_SIZE    - size of kernel chunk after decryption, set by call sfu decrypt command
 * 		BOOT1_NEEDS_FLASHING              - flag for later boot1 flashing
 * 		UBOOT_NEEDS_FLASHING            - flag for later u-boot flashing
 * 		UBOOTENV_NEEDS_FLASHING         - flag for later u-boot-env flashing
 * 		DTS_NEEDS_FLASHING              - flag for later dts flashing
 * 		ROOTFS_NEEDS_FLASHING           - flag for later rootfs flashing
 * 		KERNEL_NEEDS_FLASHING           - flag for later kernel flashing
 */

 /* TODO: generalize board_name, UBI_VOL_PREFIX, for one prefix and use it for fdt, ubi, etc... */

#define SUE_FWUPDATE_EXTRA_ENV_SETTINGS \
    "loadaddr=0x63200000\0" \
    "fdtaddr=0x63F80000\0" \
    "rdaddr=0x81000000\0" \
    "bootfile=zImage\0" \
    "cramfsdir=bcm958305k\0" \
    "board_name=mustang\0" \
    "fdtfile=\0" \
    "console=ttyS0,115200n8\0" \
    "optargs=\0" \
    "sfu_load_addr=0x63000000\0" \
    "cramfsaddr=0x63000000\0" \
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
            "sfu errstate; " \
        "fi; \0" \
    "mtdparts=" MTDPARTS_DEFAULT "\0" \
    "readuImage=" \
        "if nand read ${sfu_load_addr} kernel; " \
            "then " \
            "echo \"INFO: kernel partition load successful\"; " \
        "else " \
            "echo \"ERROR: cannot load kernel image from nand\"; " \
            "reset; " \
        "fi;\0" \
\
\
    "readDtb=" \
        "if nand read ${sfu_load_addr} dts; " \
            "then " \
            "if cramfsload ${dtb_load_addr} /${cramfsdir}/${board_name}.dtb; " \
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
                "echo \"ERROR: cannot load DTB file from cramfs\"; " \
                "reset; " \
            "fi; " \
        "else " \
            "echo \"ERROR: cannot load DTS image from nand\"; " \
            "reset; " \
        "fi;\0" \
\
\
 "nandroot=ubi0:bcm958305k-rootfs ubi.mtd=10,2048 rw\0" \
    "nandrootfstype=ubifs rootwait=1\0" \
    "nandargs=setenv bootargs console=${console} " \
        "root=${nandroot} noinitrd ${wdtargs} " \
        "rootfstype=${nandrootfstype} ${mtdparts}\0" \
    "nand_boot=echo \"Booting from nand ...\"; " \
        "run nandargs; " \
        "echo \"INFO: loading kernel image into RAM...\"; " \
        "bstate booting; " \
        "run readDtb; " \
        "run readuImage; " \
        "echo \"INFO: booting kernel image...\"; " \
        "bootz ${sfu_load_addr} - ${dtb_load_addr} ${bootargs}; " \
        "echo \"INFO: kernel boot failed...\"; " \
        "echo \"INFO: resetting...\"; " \
        "reset;\0" \
    "panicargs=panic=10 mem=512M\0" \
    "usbmount=usb reset\0" \
 \
 \
    "handle_encm=" \
        "if test ${SFU_CHNK_ENCM} = 00000001; " \
            "then " \
            "if sfu decrypt ${SFU_CHNK_DATA} ${SFU_CHNK_SIZE}; " \
                "then " \
                "echo \"INFO: chunk decrypt successful\"; " \
            "else " \
                "echo \"ERROR: chunk decrypt failed\"; " \
                "sfu errstate; " \
            "fi; " \
        "elif test ${SFU_CHNK_ENCM} = 00000000; " \
            "then " \
             "echo \"INFO: chunk not encrypted\"; " \
        "else " \
            "echo \"ERROR: encryption method invalid (${SFU_CHNK_ENCM})\"; " \
            "sfu errstate; " \
        "fi;\0" \
 \
 \
    "verify_sfu_image="\
	"if sfu valid ${sfu_load_addr}; " \
           "then " \
           "echo \"INFO: SFU image valid\"; " \
           "echo \"INFO: download partition being erased\"; " \
           "nand erase.part download; " \
           "echo \"INFO: writing sfupdate image to flash...\"; " \
           "if nand write ${sfu_load_addr} download ${filesize}; " \
               "then " \
               "echo \"INFO: nand write successful\"; " \
           "else " \
               "echo \"ERROR: nand write failed\"; " \
               "sfu errstate; " \
           "fi; " \
           "echo \"INFO: setting fail flag...\"; " \
           "fwup set fail; " \
           "SFU_IMAGE_LOAD_VALID=yes;" \
           "echo \"INFO: SFU image load and valid\"; " \
        "else echo \"INFO: SFU image invalid\"; " \
        "sfu errstate; " \
        "fi;\0" \
 \
 \
    "check_factory_state=" \
	"echo \"INFO: Checking settings,kernel,dts,rootfs,download partitions if are empty.\"; " \
	"setenv target_addr ${sfu_load_addr}; " \
	"setenv factory 1; " \
	"mw ${sfu_load_addr} 0xffffffff; " \
	"setexpr target_addr ${target_addr} + 4; " \
	"for part in settings kernel dts rootfs download; " \
	    "do; " \
	    "nand read ${target_addr} $part 4; " \
	    "cmp.l ${sfu_load_addr} ${target_addr} 1; " \
		"if test $? -eq 1; " \
		    "then; " \
		    "setenv factory 0; " \
		    "echo \"INFO: partition $part is not empty.\"; " \
		"fi; " \
	"done; " \
	"if test ${factory} -eq 0; " \
	    "then " \
	    "echo \"Board is NOT in factory state.\"; " \
	"else " \
	    "echo \"Board is in factory state.\"; " \
	"fi;\0" \
 \
 \
    "update_download_from_usb=" \
        "echo \"INFO: usb thumbdrive sfupdate check...\"; " \
        "if fatload usb ${usbdev} ${sfu_load_addr} sfupdate; " \
            "then " \
            "echo \"INFO: SFU update image found on usb thumbdrive\"; " \
	    "run verify_sfu_image;" \
        "else echo \"INFO: SFU image USB load failed\"; " \
        "fi; " \
        "echo \"INFO: usb thumbdrive sfupdate check complete\";\0" \
 \
 \
    "check_bootloaders_need_flashing=" \
        "echo \"INFO: Start check_bootloaders_need_flashing...\"; " \
        "if sfu chnkhdr ${sfu_load_addr} boot1; " \
            "then " \
            "echo \"INFO: boot1 in download partition SFU update image\"; " \
            "if test -z \\\\'${boot1_vers}\\\\' || test ${boot1_vers} != ${SFU_CHNK_VERS}; " \
                "then " \
                "if test -z \\\\'${boot1_vers}\\\\'; then echo \"INFO: boot1_vers is missing\"; fi; " \
                "if test ${boot1_vers} != ${SFU_CHNK_VERS}; then echo \"INFO: boot1_vers(${boot1_vers}) not equal SFU_CHNK_VERS(${SFU_CHNK_VERS})\"; fi; " \
                "echo \"INFO: boot1 needs flashing...\"; " \
                "run handle_encm; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_DECRYPT_BOOT1_CHNK_SIZE=${SFU_CHNK_SIZE}; " \
                "fi; " \
                "BOOT1_NEEDS_FLASHING=yes; " \
            "fi; " \
        "else " \
            "echo \"INFO: boot1 not in download partition SFU update image\"; " \
        "fi; " \
        "if sfu chnkhdr ${sfu_load_addr} u-boot; " \
            "then " \
            "echo \"INFO: uboot in download partition SFU update image\"; " \
            "if test -z \\\\'${uboot_vers}\\\\' || test ${uboot_vers} != ${SFU_CHNK_VERS}; " \
                "then " \
                "if test -z \\\\'${uboot_vers}\\\\'; then echo \"INFO: uboot_vers is missing\"; fi; " \
                "if test ${uboot_vers} != ${SFU_CHNK_VERS}; then echo \"INFO: uboot_vers(${uboot_vers}) not equal SFU_CHNK_VERS(${SFU_CHNK_VERS})\"; fi; " \
                "echo \"INFO: u-boot needs flashing...\"; " \
                "run handle_encm; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_DECRYPT_UBOOT_CHNK_SIZE=${SFU_CHNK_SIZE}; " \
                "fi; " \
                "UBOOT_NEEDS_FLASHING=yes; " \
            "fi; " \
        "else " \
            "echo \"INFO: u-boot not in download partition SFU update image\"; " \
        "fi; " \
        "if sfu chnkhdr ${sfu_load_addr} u-boot-env; " \
            "then " \
            "echo \"INFO: uboot environment in download partition SFU update image\"; " \
            "if test -z \\\\'${ubootenv_vers}\\\\' || test ${ubootenv_vers} != ${SFU_CHNK_VERS}; " \
                "then " \
                "if test -z \\\\'${ubootenv_vers}\\\\'; then echo \"INFO: ubootenv_vers is missing\"; fi; " \
                "if test ${ubootenv_vers} != ${SFU_CHNK_VERS}; then echo \"INFO: ubootenv_vers(${ubootenv_vers}) not equal SFU_CHNK_VERS(${SFU_CHNK_VERS})\"; fi; " \
                "echo \"INFO: u-boot-env needs flashing...\"; " \
                "run handle_encm; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_DECRYPT_UBOOTENV_CHNK_SIZE=${SFU_CHNK_SIZE}; " \
                "fi; " \
                "UBOOTENV_NEEDS_FLASHING=yes; " \
            "fi; " \
        "else " \
            "echo \"INFO: u-boot-env not in download partition SFU update image\"; " \
        "fi; " \
        "echo \"INFO: Completed check_bootloaders_need_flashing\";\0" \
 \
 \
    "flash_bootloaders_as_needed=" \
        "echo \"INFO: Starting flash_bootloaders_as_needed..\"; " \
        "TMP_BOOT1_VERS=${boot1_vers}; " \
        "TMP_UBOOT_VERS=${uboot_vers}; " \
        "TMP_UBOOTENV_VERS=${ubootenv_vers}; " \
        "TMP_DTS_VERS=${dts_vers}; " \
        "TMP_ROOTFS_VERS=${rootfs_vers}; " \
        "TMP_KERNEL_VERS=${kernel_vers}; " \
        "if test ${BOOT1_NEEDS_FLASHING} = yes; " \
            "then " \
            "if sfu chnkhdr ${sfu_load_addr} boot1; " \
                "then " \
                "echo \"INFO: boot1 in download partition SFU update image\"; " \
                "bstate dontunplug; " \
                "echo \"INFO: boot1 partition being erased\"; " \
                "nand erase.part ${SFU_CHNK_DEST}; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_CHNK_SIZE=${SFU_DECRYPT_BOOT1_CHNK_SIZE}; " \
                "fi; " \
                "echo \"INFO: writing boot1 to flash...\"; " \
                "if nand write ${SFU_CHNK_DATA} ${SFU_CHNK_DEST} ${SFU_CHNK_SIZE}; " \
                    "then " \
                    "echo \"INFO: nand write successful\"; " \
                "else " \
                    "echo \"ERROR: nand write failed\"; " \
                    "sfu errstate; " \
                "fi; " \
                "TMP_BOOT1_VERS=${SFU_CHNK_VERS}; " \
                "UBOOT_RESET=yes; " \
            "else " \
                "echo \"ERROR: boot1 not in download partition SFU update image\"; " \
                "sfu errstate; " \
            "fi; " \
        "fi; " \
        "if test ${UBOOT_NEEDS_FLASHING} = yes; " \
            "then " \
            "if sfu chnkhdr ${sfu_load_addr} u-boot; " \
                "then " \
                "echo \"INFO: uboot in download partition SFU update image\"; " \
                "bstate dontunplug; " \
                "echo \"INFO: u-boot partition being erased\"; " \
                "nand erase.part ${SFU_CHNK_DEST}; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_CHNK_SIZE=${SFU_DECRYPT_UBOOT_CHNK_SIZE}; " \
                "fi; " \
                "echo \"INFO: writing uboot to flash...\"; " \
                "if nand write ${SFU_CHNK_DATA} ${SFU_CHNK_DEST} ${SFU_CHNK_SIZE}; " \
                    "then " \
                    "echo \"INFO: nand write successful\"; " \
                "else " \
                    "echo \"ERROR: nand write failed\"; " \
                    "sfu errstate; " \
                "fi; " \
                "TMP_UBOOT_VERS=${SFU_CHNK_VERS}; " \
                "UBOOT_RESET=yes; " \
            "else " \
                "echo \"ERROR: u-boot not in download partition SFU update image\"; " \
                "sfu errstate; " \
            "fi; " \
        "fi; " \
        "if test ${UBOOTENV_NEEDS_FLASHING} = yes; " \
            "then " \
            "if sfu chnkhdr ${sfu_load_addr} u-boot-env; " \
                "then " \
                "echo \"INFO: uboot environment in download partition SFU update image\"; " \
                "bstate dontunplug; " \
                "echo \"INFO: u-boot-env partition being erased\"; " \
                "nand erase.part ${SFU_CHNK_DEST}; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_CHNK_SIZE=${SFU_DECRYPT_UBOOTENV_CHNK_SIZE}; " \
                "fi; " \
                "echo \"INFO: writing uboot-env to flash...\"; " \
                "if nand write ${SFU_CHNK_DATA} ${SFU_CHNK_DEST} ${SFU_CHNK_SIZE}; " \
                    "then " \
                    "echo \"INFO: nand write successful\"; " \
                "else " \
                    "echo \"ERROR: nand write failed\"; " \
                    "sfu errstate; " \
                "fi; " \
                "echo \"INFO: reloading uboot-env from flash to internal memory so new environment is used...\"; " \
                "env reload;" \
                "TMP_UBOOTENV_VERS=${SFU_CHNK_VERS};" \
                "UBOOT_RESET=yes; " \
            "else " \
                "echo \"ERROR: u-boot-env not in download partition SFU update image\"; " \
                "sfu errstate; " \
            "fi; " \
        "fi; " \
        "if test ${BOOT1_NEEDS_FLASHING} = yes; " \
            "then " \
            "echo \"INFO: setting boot1_vers in environment...\"; " \
            "setenv boot1_vers ${TMP_BOOT1_VERS};" \
        "fi; " \
        "if test ${UBOOT_NEEDS_FLASHING} = yes; " \
            "then " \
            "echo \"INFO: setting uboot_vers in environment...\"; " \
            "setenv uboot_vers ${TMP_UBOOT_VERS};" \
        "fi; " \
        "if test ${UBOOTENV_NEEDS_FLASHING} = yes; " \
            "then " \
            "echo \"INFO: setting boot1 in environment...\"; " \
            "setenv boot1_vers ${TMP_BOOT1_VERS};" \
            "echo \"INFO: setting uboot_vers in environment...\"; " \
            "setenv uboot_vers ${TMP_UBOOT_VERS};" \
            "echo \"INFO: setting ubootenv_vers in environment...\"; " \
            "setenv ubootenv_vers ${TMP_UBOOTENV_VERS};" \
            "echo \"INFO: restoring dts_vers in environment...\"; " \
            "setenv dts_vers ${TMP_DTS_VERS};" \
            "echo \"INFO: restoring rootfs_vers in environment...\"; " \
            "setenv rootfs_vers ${TMP_ROOTFS_VERS};" \
            "echo \"INFO: restoring kernel_vers in environment...\"; " \
            "setenv kernel_vers ${TMP_KERNEL_VERS};" \
        "fi; " \
        "if test ${BOOT1_NEEDS_FLASHING} = yes || test ${UBOOT_NEEDS_FLASHING} = yes || test ${UBOOTENV_NEEDS_FLASHING} = yes; " \
            "then " \
            "echo \"INFO: saving environment...\"; " \
            "saveenv;" \
        "fi; " \
        "echo \"INFO: Completed flash_bootloaders_as_needed.\";\0" \
 \
 \
    "check_system_need_flashing=" \
        "echo \"INFO: Start check_system_need_flashing...\"; " \
        "if sfu chnkhdr ${sfu_load_addr} dts; " \
            "then " \
            "echo \"INFO: dts in download partition SFU update image\"; " \
            "if fwup fail; then echo \"INFO: Fail flag is set\"; fi; " \
            "if test -z \\\\'${dts_vers}\\\\'; then echo \"INFO: dts_vers is missing\"; fi; " \
            "if test ${dts_vers} != ${SFU_CHNK_VERS}; then echo \"INFO: dts_vers(${dts_vers}) not equal SFU_CHNK_VERS(${SFU_CHNK_VERS})\";  fi; " \
            "if fwup fail ||  test -z \\\\'${dts_vers}\\\\' || test ${dts_vers} != ${SFU_CHNK_VERS}; " \
                "then " \
                "echo \"INFO: dts needs flashing...\"; " \
                "run handle_encm; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_DECRYPT_DTS_CHNK_SIZE=${SFU_CHNK_SIZE}; " \
                "fi; " \
                "DTS_NEEDS_FLASHING=yes; " \
            "fi; " \
        "else " \
            "echo \"INFO: dts not in download partition SFU update image\"; " \
        "fi; " \
        "if sfu chnkhdr ${sfu_load_addr} rootfs; " \
            "then " \
            "echo \"INFO: rootfs in download partition SFU update image\"; " \
            "if fwup fail; then echo \"INFO: Fail flag is set\"; fi; " \
            "if test -z \\\\'${rootfs_vers}\\\\'; then echo \"INFO: rootfs_vers is missing\"; fi; " \
            "if test ${rootfs_vers} != ${SFU_CHNK_VERS}; then echo \"INFO: rootfs_vers(${rootfs_vers}) not equal SFU_CHNK_VERS(${SFU_CHNK_VERS})\";  fi; " \
            "if fwup fail ||  test -z \\\\'${rootfs_vers}\\\\' || test ${rootfs_vers} != ${SFU_CHNK_VERS}; " \
                "then " \
                "echo \"INFO: rootfs needs flashing...\"; " \
                "run handle_encm; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_DECRYPT_ROOTFS_CHNK_SIZE=${SFU_CHNK_SIZE}; " \
                "fi; " \
                "ROOTFS_NEEDS_FLASHING=yes; " \
            "fi; " \
        "else " \
            "echo \"INFO: rootfs not in download partition SFU update image\"; " \
        "fi; " \
        "if sfu chnkhdr ${sfu_load_addr} kernel; " \
            "then " \
            "echo \"INFO: kernel in download partition SFU update image\"; " \
            "if fwup fail; then echo \"INFO: Fail flag is set\"; fi; " \
            "if test -z \\\\'${kernel_vers}\\\\'; then echo \"INFO: kernel_vers is missing\"; fi; " \
            "if test ${kernel_vers} != ${SFU_CHNK_VERS}; then echo \"INFO: kernel_vers(${kernel_vers}) not equal SFU_CHNK_VERS(${SFU_CHNK_VERS})\";  fi; " \
            "if fwup fail ||  test -z \\\\'${kernel_vers}\\\\' || test ${kernel_vers} != ${SFU_CHNK_VERS}; " \
                "then " \
                "echo \"INFO: kernel needs flashing...\"; " \
                "run handle_encm; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_DECRYPT_KERNEL_CHNK_SIZE=${SFU_CHNK_SIZE}; " \
                "fi; " \
                "KERNEL_NEEDS_FLASHING=yes; " \
            "fi; " \
        "else " \
            "echo \"INFO: kernel not in download partition SFU update image\"; " \
        "fi; " \
        "echo \"INFO: Completed check_system_need_flashing\";\0" \
 \
 \
    "flash_system_as_needed=" \
        "echo \"INFO: Starting flash_system_as_needed...\"; " \
        "if test ${DTS_NEEDS_FLASHING} = yes || test ${ROOTFS_NEEDS_FLASHING} = yes || test ${KERNEL_NEEDS_FLASHING} = yes; " \
            "then " \
            "echo \"INFO: kernel partition being erased\"; " \
            "nand erase.part kernel; " \
            "KERNEL_NEEDS_FLASHING=yes; " \
        "fi; " \
        "if test ${DTS_NEEDS_FLASHING} = yes; " \
            "then " \
            "if sfu chnkhdr ${sfu_load_addr} dts; " \
                "then " \
                "echo \"INFO: dts in download partition SFU update image\"; " \
                "bstate dontunplug; " \
                "echo \"INFO: dts partition being erased\"; " \
                "nand erase.part ${SFU_CHNK_DEST}; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_CHNK_SIZE=${SFU_DECRYPT_DTS_CHNK_SIZE}; " \
                "fi; " \
                "echo \"INFO: writing dts to flash...\"; " \
                "if nand write ${SFU_CHNK_DATA} ${SFU_CHNK_DEST} ${SFU_CHNK_SIZE}; " \
                    "then " \
                    "echo \"INFO: nand write successful\"; " \
                "else " \
                    "echo \"ERROR: nand write failed\"; " \
                    "sfu errstate; " \
                "fi; " \
                "setenv dts_vers ${SFU_CHNK_VERS};" \
            "fi; " \
        "fi; " \
        "if test ${ROOTFS_NEEDS_FLASHING} = yes; " \
            "then " \
            "if sfu chnkhdr ${sfu_load_addr} rootfs; " \
                "then " \
                "echo \"INFO: rootfs in download partition SFU update image\"; " \
                "bstate dontunplug; " \
                "echo \"INFO: erasing rootfs partition\"; " \
                "nand erase.part rootfs; " \
                "if ubi part rootfs 2048; " \
                    "then " \
                    "echo \"INFO: rootfs is a valid ubi partition\"; " \
                "fi; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_CHNK_SIZE=${SFU_DECRYPT_ROOTFS_CHNK_SIZE}; " \
                "fi; " \
                "echo \"INFO: need to create ubi ${UBI_VOL_PREFIX}-rootfs volume first...\"; " \
                "if ubi create ${UBI_VOL_PREFIX}-rootfs; " \
                    "then " \
                    "echo \"INFO: successfully created volume, ${UBI_VOL_PREFIX}-rootfs...\"; " \
                    "echo \"INFO: writing ubi rootfs to flash...\"; " \
                    "if ubi write ${SFU_CHNK_DATA} ${UBI_VOL_PREFIX}-rootfs ${SFU_CHNK_SIZE}; " \
                        "then " \
                        "echo \"INFO: successfully wrote rootfs to flash after volume, ${UBI_VOL_PREFIX}-rootfs, was created...\"; " \
                    "else " \
                        "echo \"ERROR: failed to write rootfs to flash after volume, ${UBI_VOL_PREFIX}-rootfs, was created\"; " \
                        "sfu errstate; " \
                    "fi; " \
                "else " \
                    "echo \"ERROR: failed to create volume, ${UBI_VOL_PREFIX}-rootfs\"; " \
                    "sfu errstate; " \
                "fi; " \
                "setenv rootfs_vers ${SFU_CHNK_VERS};" \
            "else " \
                "echo \"ERROR: rootfs not in download partition SFU update image\"; " \
                "sfu errstate; " \
            "fi; " \
        "fi; " \
        "if test ${KERNEL_NEEDS_FLASHING} = yes; " \
            "then " \
            "if sfu chnkhdr ${sfu_load_addr} kernel; " \
                "then " \
                "echo \"INFO: kernel in download partition SFU update image\"; " \
                "bstate dontunplug; " \
                "if test ${SFU_CHNK_ENCM} = 00000001; " \
                    "then " \
                    "SFU_CHNK_SIZE=${SFU_DECRYPT_KERNEL_CHNK_SIZE}; " \
                "fi; " \
                "echo \"INFO: writing kernel to flash...\"; " \
                "if nand write ${SFU_CHNK_DATA} ${SFU_CHNK_DEST} ${SFU_CHNK_SIZE}; " \
                    "then " \
                    "echo \"INFO: nand write successful\"; " \
                "else " \
                    "echo \"ERROR: nand write failed\"; " \
                    "sfu errstate; " \
                "fi; " \
                "setenv kernel_vers ${SFU_CHNK_VERS};" \
            "else " \
                "echo \"ERROR: kernel not in download partition SFU update image\"; " \
                "sfu errstate; " \
            "fi; " \
        "fi; " \
        "if test ${DTS_NEEDS_FLASHING} = yes || test ${ROOTFS_NEEDS_FLASHING} = yes || test ${KERNEL_NEEDS_FLASHING} = yes; " \
            "then " \
            "echo \"INFO: saving environment...\"; " \
            "saveenv;" \
        "fi; " \
        "echo \"INFO: Completed flash_system_as_needed.\";\0" \
 \
 \
    "sfu_boot=" \
        "echo \"INFO: SFU firmware update process started...\"; " \
        "SFU_IMAGE_LOAD_VALID=no; " \
        "if fwup fail; " \
            "then " \
            "if test ${bootcount} -eq ${bootlimit}; " \
                "then " \
                "echo \"INFO: bootcount(${bootcount}) equals bootlimit(${bootlimit})\"; " \
                "echo \"INFO: erasing settings partition\"; " \
                "run eraseSettings; " \
                "if ubi part settings 2048; " \
                    "then " \
                    "echo \"INFO: settings is a valid ubi partition\"; " \
                    "echo \"INFO: creating volume settings...\"; " \
                    "if ubi create ${UBI_VOL_PREFIX}-settings; " \
                        "then " \
                        "echo \"INFO: successfully created volume, settings...\"; " \
                    "else " \
                        "echo \"ERROR: failed to create volume, settings\"; " \
                        "sfu errstate; " \
                    "fi; " \
                "fi; " \
            "fi; " \
            "if test ${bootcount} -gt ${bootlimit}; " \
                "then " \
                "echo \"INFO: bootcount(${bootcount}) greater than bootlimit(${bootlimit})\"; " \
                "bstate hardfailure; " \
                "echo \"ERROR: Maximum boot count reached!\"; " \
                "sfu errstate; " \
            "fi; " \
        "fi; " \
        "if test ${bootcount} -eq 1; " \
            "then " \
	    "run check_factory_state; " \
            "if fwup usb_update_req || test ${factory} -eq 1; " \
                "then " \
                "echo \"INFO: USB update request active, checking USB for update file ...\"; " \
                "run usbmount; " \
                "usbdev=-1; " \
                "if usb storage; " \
                    "then " \
                    "if fatfind usb 0 / sfupdate; " \
                        "then " \
                        "echo \"INFO: sfupdate image found on USB-0 thumbdrive\"; " \
                        "bstate dontunplug; " \
                        "usbdev=0; " \
                        "run update_download_from_usb; " \
                    "else " \
			 "if fatfind usb 1 / sfupdate; "\
			    "then " \
			    "echo \"INFO: sfupdate image found on USB-1 thumbdrive\"; " \
			    "bstate dontunplug; " \
			    "usbdev=1; " \
			    "run update_download_from_usb; " \
			"else " \
                            "echo \"ERROR: sfupdate image not found on any USB thumbdrive\"; " \
                            "sfu errstate; " \
			"fi;" \
		    "fi; " \
		    "else "\
		    "setenv autoload no; " \
		    "setenv autostart no; " \
		    "echo \"INFO: starting tftp update process.\"; " \
		    "if stftpup data; " \
			"then "\
			"if test -z \\\\'${serverip}\\\\'; then " \
			    "stftpup use_local_ip; " \
			    "echo \"INFO: Serverip unknown. Using local ip with last digit .10\"; " \
			"fi; " \
		    "else " \
			"echo \"DHCP failed, use static settings\"; " \
			"stftpup use_static_data; " \
		    "fi; " \
			\
		    "if test -z \\\\'${bootfile}\\\\' || test ${bootfile} = uImage; then " \
			"setenv bootfile sfupdate; " \
		    "fi; " \
			\
		    "if tftp ${sfu_load_addr} ${rootpath}${bootfile}; " \
			"then " \
			"run verify_sfu_image; " \
		    "else " \
			"echo \"INFO: Cannot fetch update file\"; " \
		    "fi; " \
                "fi; " \
            "else " \
                "echo \"INFO: USB update request is not active, not checking USB for FW update file\" ;" \
            "fi; " \
        "else " \
	        "echo \"INFO: Bootcount != 1, not checking USB for FW update file\" ;" \
        "fi; " \
        "if fwup fail || fwup update || test -z \\\\'${boot1_vers}\\\\' || test -z \\\\'${uboot_vers}\\\\' || test -z \\\\'${ubootenv_vers}\\\\' || test -z \\\\'${dts_vers}\\\\' || test -z \\\\'${rootfs_vers}\\\\' || test -z \\\\'${kernel_vers}\\\\'; " \
            "then " \
            "if fwup fail; then echo \"INFO: Fail flag is set\"; fi; " \
            "if fwup update; then echo \"INFO: Update flag is set\"; fi; " \
            "if test -z \\\\'${boot1_vers}\\\\'; then echo \"INFO: boot1_vers is missing\"; fi; " \
            "if test -z \\\\'${uboot_vers}\\\\'; then echo \"INFO: uboot_vers is missing\"; fi; " \
            "if test -z \\\\'${ubootenv_vers}\\\\'; then echo \"INFO: ubootenv_vers is missing\"; fi; " \
            "if test -z \\\\'${dts_vers}\\\\'; then echo \"INFO: dts_vers is missing\"; fi; " \
            "if test -z \\\\'${kernel_vers}\\\\'; then echo \"INFO: kernel_vers is missing\"; fi; " \
            "if test -z \\\\'${rootfs_vers}\\\\'; then echo \"INFO: rootfs_vers is missing\"; fi; " \
            "bstate dontunplug; " \
            "VALID_IMAGE=no; " \
            "if test ${SFU_IMAGE_LOAD_VALID} = yes; " \
                "then " \
                "VALID_IMAGE=yes; " \
            "else " \
                "echo \"INFO: Need to read the sfupdate from download partition...\"; " \
                "if nand read ${sfu_load_addr} download 0x10; " \
                    "then " \
                    "if sfu magic ${sfu_load_addr}; " \
                        "then " \
                        "echo \"INFO: sfupdate image total length is ${SFU_TOTAL_LEN}\"; " \
                        "if nand read ${sfu_load_addr} download ${SFU_TOTAL_LEN}; " \
                            "then " \
                            "if sfu valid ${sfu_load_addr}; " \
                                "then " \
                                "echo \"INFO: SFU image in download partition is valid\"; " \
                                "VALID_IMAGE=yes; " \
                            "else " \
                                "echo \"ERROR: SFU image in download partition is invalid\"; " \
                            "fi; " \
                        "else " \
                            "echo \"ERROR: nand read failed\"; " \
                        "fi; " \
                    "else " \
                        "echo \"ERROR: SFU image in download partition is invalid\"; " \
                    "fi; " \
                "else " \
                    "echo \"ERROR: nand read failed\"; " \
                "fi; " \
            "fi; " \
            "if test ${VALID_IMAGE} = yes; " \
                "then " \
                "UBOOT_NEEDS_FLASHING=no; " \
                "UBOOTENV_NEEDS_FLASHING=no; " \
                "BOOT1_NEEDS_FLASHING=no; " \
                "DTS_NEEDS_FLASHING=no; " \
                "ROOTFS_NEEDS_FLASHING=no; " \
                "KERNEL_NEEDS_FLASHING=no; " \
                "UBOOT_RESET=no; " \
                "run check_bootloaders_need_flashing; " \
                "run flash_bootloaders_as_needed; " \
                "if test ${UBOOT_RESET} = yes; " \
                "then " \
                    "echo \"INFO: Resetting ...\"; " \
                    "reset; " \
                "fi;" \
                "run check_system_need_flashing; " \
                "run flash_system_as_needed; " \
             "else " \
                "if fwup fail || test -z \\\\'${boot1_vers}\\\\' || test -z \\\\'${uboot_vers}\\\\' || test -z \\\\'${ubootenv_vers}\\\\' || test -z \\\\'${dts_vers}\\\\' || test -z \\\\'${rootfs_vers}\\\\' || test -z \\\\'${kernel_vers}\\\\'; " \
                "then " \
                    "sfu errstate; " \
                "fi; " \
             "fi; " \
        "fi; " \
        "echo \"INFO: SFU firmware update process completed\"; " \
        "echo \"INFO: clearing update flag...\"; " \
        "fwup clear update; " \
        "echo \"INFO: setting fail flag...\"; " \
        "fwup set fail; " \
        "bstate normal; " \
        "run nand_boot;\0" \

#define SUE_FWUPDATE_BCM_BOOTCOMMAND \
	"echo \"INFO: attempting SFU boot...\"; " \
	"echo \"INFO: boot1 version: ${boot1_vers}\"; " \
	"echo \"INFO: U-boot version: ${uboot_vers}\"; " \
	"echo \"INFO: zImage version: ${kernel_vers}\"; " \
	"echo \"INFO: Rootfs version: ${rootfs_vers}\"; " \
	"fwup flags; "\
	"run sfu_boot;" \

#define SUE_FWUPDATE_BCM_ALTBOOTCOMMAND \
	"echo \"ERROR: Maximum boot count reached!\"; sfu errstate;"

#endif /* __SUE_FWUPDATE_BCM_H */
