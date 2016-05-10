/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#ifndef PERSISTENT_INFO_H
#define PERSISTENT_INFO_H

/*
 * EP-wide interface to get CPU reset reason.
 * Default weak implementation in board.c is a stub that returns
 * "UNIMPLEMENTED". CPU support code can override this.
 */
int get_reset_reason(void);
unsigned long get_ramdump_reason(void);
void clear_ramdump_reason(unsigned long val);

#endif /* PERSISTENT_INFO_H */
