/*****************************************************************************
 * Copyright 2014 - 2015 Broadcom Corporation.  All rights reserved.
 *
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2, available at
 * http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a
 * license other than the GPL, without Broadcom's express prior written
 * consent.
 *****************************************************************************/
#include <common.h>
#include <asm/io.h>
#include <asm/persistent_info.h>
#ifdef CONFIG_USBDOWNLOAD_GADGET
#include <g_dnl.h>
#endif

/* Ramdump reason */
#define UPLOAD_CAUSE_INIT		0xCAFEBABE
#define UPLOAD_CAUSE_KERNEL_PANIC	0x000000C8
#define UPLOAD_CAUSE_FORCED_UPLOAD	0x00000022
#define UPLOAD_CAUSE_USER_FAULT		0x0000002F
#define NORMAL_BOOT			0xBABEFACE
#define UPLOAD_FAILED			0xDEADBEEF

#ifdef CONFIG_USBDOWNLOAD_GADGET
static int start_ramdump_upload(void)
{
	int ret = -1;
	unsigned long ramdump_upload_done = UPLOAD_FAILED;

	g_dnl_clear_detach();
	ret = g_dnl_register("usb_dnl_fastboot");
	if (ret)
		return ret;

	while (1) {
		if (g_dnl_detach()) {
			ret = -1;
			break;
		}
		if (ctrlc()) {
			ret = -1;
			break;
		}
		ramdump_upload_done =
			getenv_ulong("ramdump_upload_state", 10, 0);
		if (ramdump_upload_done == 1) {
			ret = 0;
			break;
		}
		usb_gadget_handle_interrupts();
	}

	g_dnl_unregister();
	g_dnl_clear_detach();

	return ret;
}
#else /* CONFIG_USBDOWNLOAD_GADGET */
static int start_ramdump_upload(void)
{
	return 0;
}
#endif

int find_ramdump_reason(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	unsigned long ramdump_reason = get_ramdump_reason();
	int ret = 0;

	if (ramdump_reason == UPLOAD_CAUSE_KERNEL_PANIC ||
	    ramdump_reason == UPLOAD_CAUSE_FORCED_UPLOAD ||
	    ramdump_reason == UPLOAD_CAUSE_USER_FAULT) {
		printf("Detected Ramdump mode cause = 0x%08x\n",
		       ramdump_reason);
		ret = start_ramdump_upload();
		if (!ret) {
			printf("Ramdump upload success\n");
			clear_ramdump_reason(NORMAL_BOOT);
		} else {
			printf("Ramdump upload stopped/interrupted\n");
			printf("run command ramdump to enter ramdump mode\n");
		}
	} else {
		printf("System is not in Ramdump upload mode\n");
	}

	return 0;
}

U_BOOT_CMD(
	   ramdump, 2, 1, find_ramdump_reason,
	   "Find ramdump reason and do accordingly.",
	   "run ramdump\n"
	  );

