/*****************************************************************************
* Copyright 2006 - 2013 Broadcom Corporation.  All rights reserved.
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

#ifndef _BCMCAPRI_NAND_H
#define _BCMCAPRI_NAND_H

// #define BCMCAPRI_NAND_DEBUG

#ifdef BCMCAPRI_NAND_DEBUG
#include <exports.h>
#define dbgprint(fmt, ...) \
	printf(fmt, ##__VA_ARGS__)
#else
#define dbgprint(fmt, ...)
#endif

#endif				/* _BCMCAPRI_NAND_H */
