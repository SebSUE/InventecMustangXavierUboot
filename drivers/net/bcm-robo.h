/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _bcm_robo_h_
#define _bcm_robo_h_

#include <linux/types.h>

/* MPORT values. For adding ARL entries */
#define MPORT_RESERVED	0
#define MPORT_START	1
#define MPORT_LAST	5
#define MPORT_TOTAL	(MPORT_LAST - MPORT_START + 1)

/* Device access/config oprands */
struct esw_ops {
	int (*read_reg)(void *esw, uint8_t page, uint8_t offset,
			void *val, int len);
	int (*write_reg)(void *esw, uint8_t page, uint8_t offset,
			 void *val, int len);
};

/* Private state per RoboSwitch */
struct esw_info {
	void *srab_base;
	struct esw_ops *ops;		/* device ops */
	uint32_t devid32;		/* Device id for the switch (32bits) */
	uint32_t corerev;		/* Core rev of internal switch */
};

int bcm_esw_init(void *privp);
void bcm_esw_write_rst_reason(void *privp, u32 rst_seq);

#endif /* _bcm_robo_h_ */
