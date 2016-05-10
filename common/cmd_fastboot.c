/*
 * Copyright 2008 - 2009 Windriver, <www.windriver.com>
 * Author: Tom Rix <Tom.Rix@windriver.com>
 *
 * (C) Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <command.h>
#include <fastboot.h>
#include <g_dnl.h>

static int do_fastboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;
	char ubootcmd[100];
	char *s;
	g_dnl_clear_detach();
	printf("fastboot cmd started...\n");
	s = getenv("flashdevice");
	if ((s != NULL) && (strcmp(s, "emmc") == 0)) {
		printf("emmc has selected as flash device\n");
		printf("for nand: setenv flashdevice nand;saveenv");
		fastboot_discover_gpt_tables();
	} else {
		printf("nand has selected as flash device\n");
		printf("for emmc: setenv flashdevice emmc;saveenv");
#ifdef CONFIG_EFI_PARTITION
		sprintf(ubootcmd, "mtdparts setenv gpt");
		if (run_command(ubootcmd, 0) == 0) {
			sprintf(ubootcmd,
				"nand read %x ${mtdparts_addr} ${mtdparts_size}",
				CONFIG_USB_FASTBOOT_BUF_ADDR);
			if (run_command(ubootcmd, 0) == 0) {
				sprintf(ubootcmd, "printgpt %x",
					CONFIG_USB_FASTBOOT_BUF_ADDR);
				if (run_command(ubootcmd, 0)) {
					sprintf(ubootcmd, "mtdparts delall");
					run_command(ubootcmd, 0);
					sprintf(ubootcmd, "saveenv");
					run_command(ubootcmd, 0);
				}
			} else {
				return CMD_RET_FAILURE;
			}
		} else {
			sprintf(ubootcmd, "mtdparts delall");
			run_command(ubootcmd, 0);
			sprintf(ubootcmd, "saveenv");
			run_command(ubootcmd, 0);
		}
#else
		run_command("mtdparts", 0);
#endif
	}

	ret = g_dnl_register("usb_dnl_fastboot");
	if (ret)
		return ret;

	while (1) {
		if (g_dnl_detach())
			break;
		if (ctrlc())
			break;
		usb_gadget_handle_interrupts();
	}

	g_dnl_unregister();
	g_dnl_clear_detach();
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	fastboot,	1,	1,	do_fastboot,
	"fastboot - enter USB Fastboot protocol",
	""
);
static int do_printgpt(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long offset;
	offset = simple_strtoul(argv[1], NULL, 16);
	if (offset) {
		if (print_nand_part_efi(offset)) {
			printf("Invalid GPT exist\n");
			printf("Please update New gpt.bin\n");
			return CMD_RET_FAILURE;
		}
	} else {
		printf("ERROR: enter RAM ADDR");
	}
	return CMD_RET_SUCCESS;
}
U_BOOT_CMD(
	printgpt,	2,	1,	do_printgpt,
	"printgpt <RAM ADDR>- print gpt",
	"RAM ADDR- gpt.bin should be load here\n"
	"update the mtdparts with gpt info"
);
