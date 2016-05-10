/*****************************************************************************
*
* Copyright 2013 Broadcom Corporation.  All rights reserved.
*
* SPDX-License-Identifier:      GPL-2.0+
*
*****************************************************************************/

#include <common.h>
#include <asm/io.h>
#include <asm/kona-common/misc.h>
#include <asm/armv7.h>

void do_proc_wfe(int forever)
{
	do {
		asm volatile ("wfe	@ wait for event\n");
		/*printf("wake up from wfe\n"); */
	} while (forever);
}
