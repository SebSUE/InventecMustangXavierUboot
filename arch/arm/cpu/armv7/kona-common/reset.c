/*****************************************************************************
*
* Copyright 2013 Broadcom Corporation.  All rights reserved.
*
* SPDX-License-Identifier:      GPL-2.0+
*
*****************************************************************************/

#include <common.h>
#include <asm/io.h>
#include <asm/kona-common/reset.h>

void reset_cpu(ulong addr)
{
	/* Call architecture-specific implementation */
	kona_chip_reset(addr);
}
