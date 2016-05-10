/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>

#define RESET_CPU_LEVEL3 0xA5A50003

int do_reset_l3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	puts("resetting ...\n");

	/* wait 50 ms */
	udelay(50000);

	disable_interrupts();

	reset_misc();
	reset_cpu(RESET_CPU_LEVEL3);

	/*NOTREACHED*/
	return 0;
}


U_BOOT_CMD(
	reset_l3, 1, 0, do_reset_l3,
	"Perform L3 RESET of the CPU",
	""
);
