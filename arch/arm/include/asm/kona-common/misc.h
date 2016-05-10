/*****************************************************************************
*
* Copyright 2013 Broadcom Corporation.  All rights reserved.
*
* SPDX-License-Identifier:      GPL-2.0+
*
*****************************************************************************/

#ifndef __KONA_COMMON_MISC_H
#define __KONA_COMMON_MISC_H

#include <mmc.h>

void hw_watchdog_disable(void);
int kona_sdhci_init(int dev_index, u32 min_clk, u32 quirks);
int kona_i2c_init(unsigned int dev_idx, unsigned long base_addr);

void proc_on_wfi(void);

#endif
