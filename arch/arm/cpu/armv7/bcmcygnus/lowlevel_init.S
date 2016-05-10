/*
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef CONFIG_SKIP_LOWLEVEL_INIT

.globl lowlevel_init
lowlevel_init:
	mov	pc, lr

#endif /* #ifndef CONFIG_SKIP_LOWLEVEL_INIT */
