/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#define SRAM_GEN_DEFS
#include <common.h>
#include <asm/io.h>
#include <asm/arch/sram_globals.h>
#include <asm/persistent_info.h>

#define RESET_CPU_LEVEL3	0xA5A50003

#define CRMU_MAIL_BOX0		0x03024024
#define CRMU_MAIL_BOX1		0x03024028
#define CRMU_SOFT_RESET_L3_CMD0	0x00000003
#define CRMU_SOFT_RESET_L0_CMD0	0x00000005
#define CRMU_SOFT_RESET_CMD1	0x00000000

void reset_cpu(ulong level)
{
	/* Send soft reset command via Mailbox. */
	if (level == RESET_CPU_LEVEL3)
		writel(CRMU_SOFT_RESET_L3_CMD0, CRMU_MAIL_BOX0);
	else
		writel(CRMU_SOFT_RESET_L0_CMD0, CRMU_MAIL_BOX0);
	writel(CRMU_SOFT_RESET_CMD1, CRMU_MAIL_BOX1);

	while (1)
		;	/* loop forever till reset */
}

int get_reset_reason(void)
{
	struct PERSISTENT_GLOBALS *eg = PERSISTENT_GLOBALS_PTR();
	int reason = eg->reset.reason;

	if (reason >= RESET_REASON_MAX) {
		error("Invalid reset reason (%d)!\n", reason);
		reason = RESET_REASON_UNKNOWN;
	}

	return reason;
}

unsigned long get_ramdump_reason(void)
{
	struct PERSISTENT_GLOBALS *eg = PERSISTENT_GLOBALS_PTR();

	return eg->ramdump_reason;
}

void clear_ramdump_reason(unsigned long val)
{
	struct PERSISTENT_GLOBALS *eg = PERSISTENT_GLOBALS_PTR();

	eg->ramdump_reason = val;
}
