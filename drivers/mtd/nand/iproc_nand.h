/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#ifndef _IPROC_NAND_H_
#define _IPROC_NAND_H_

#include <config.h>

/*
 * SoC specific definitions
 */
#if defined(CONFIG_CYGNUS)

#define REG_NAND_BASE				0x18046000
#define REG_NAND_IDM_BASE			0xf8105000

#define NAND_STRAP_TYPE_MASK			0x000f0000
#define NAND_STRAP_TYPE_SHIFT			16
#define NAND_STRAP_PAGE_MASK			0x00300000
#define NAND_STRAP_PAGE_SHIFT			20
#define NAND_STRAP_WIDTH_MASK			0x01000000
#define NAND_STRAP_WIDTH_SHIFT			24

#define NAND_STRAP_TYPE_DATA			\
/* sector_1k, ecclevel, spare_size */		\
{						\
	{ 0,  0, 16 },				\
	{ 0,  1, 16 },				\
	{ 0,  4, 16 },				\
	{ 0,  8, 16 },				\
	{ 0,  8, 27 },				\
	{ 0, 12, 27 },				\
	{ 1, 12, 27 },				\
	{ 1, 15, 27 },				\
	{ 1, 20, 45 }				\
}

#define NAND_STRAP_PAGE_DATA			\
{						\
	1024, 2048, 4096, 8192			\
}

/*
 * iProc NAND timing configurations for ONFI timing modes [0-5]
 *
 * Clock tick = 10ns
 * Multiplier:
 * x1: tWP tWH tRP tREH tCLH tALH
 * x2: tCS tADL tWB tWHR
 */
#define NAND_TIMING_DATA					\
{								\
	/* ONFI timing mode 0 :				*/	\
	/* tWC=100ns tWP=50ns tWH=30ns			*/	\
	/* tRC=100ns tRP=50ns tREH=30ns			*/	\
	/* tCS=70ns tCLH=20ns tALH=20ns tADL=200ns	*/	\
	/* tWB=200ns tWHR=120ns tREA=40ns		*/	\
	{							\
		.timing1 = 0x6565435b,				\
		.timing2 = 0x00001e85,				\
	},							\
	/* ONFI timing mode 1 :				*/	\
	/* tWC=45 tWP=25ns tWH=15ns			*/	\
	/* tRC=50 tRP=25ns tREH=15ns			*/	\
	/* tCS=35ns tCLH=10ns tALH=10ns tADL=100ns	*/	\
	/* tWB=100ns tWHR=80ns tREA=30ns		*/	\
	{							\
		.timing1 = 0x33333236,				\
		.timing2 = 0x00001064,				\
	},							\
	/* ONFI timing mode 2 :				*/	\
	/* tWC=35ns tWP=17ns tWH=15ns			*/	\
	/* tRC=35ns tRP=17ns tREH=15ns			*/	\
	/* tCS=25ns tCLH=10ns tALH=10ns tADL=100ns	*/	\
	/* tWB=100ns tWHR=80ns tREA=25ns		*/	\
	{							\
		.timing1 = 0x32322226,				\
		.timing2 = 0x00001063,				\
	},							\
	/* ONFI timing mode 3 :				*/	\
	/* tWC=30ns tWP=15ns tWH=10ns			*/	\
	/* tRC=30ns tRP=15ns tREH=10ns			*/	\
	/* tCS=25ns tCLH=5ns tALH=5ns tADL=100ns	*/	\
	/* tWB=100ns tWHR=60ns tREA=20ns		*/	\
	{							\
		.timing1 = 0x22222126,				\
		.timing2 = 0x00001043,				\
	},							\
	/* ONFI timing mode 4 :				*/	\
	/* tWC=25ns tWP=12ns tWH=10ns			*/	\
	/* tRC=25ns tRP=12ns tREH=10ns			*/	\
	/* tCS=20ns tCLH=5ns tALH=5ns tADL=70ns		*/	\
	/* tWB=100ns tWHR=60ns tREA=20ns		*/	\
	{							\
		.timing1 = 0x21212114,				\
		.timing2 = 0x00001042,				\
	},							\
	/* ONFI timing mode 5 :				*/	\
	/* tWC=20ns tWP=10ns tWH=7ns			*/	\
	/* tRC=20ns tRP=10ns tREH=7ns			*/	\
	/* tCS=15ns tCLH=5ns tALH=5ns tADL=70ns		*/	\
	/* tWB=100ns tWHR=60ns tREA=16ns		*/	\
	{							\
		.timing1 = 0x11111114,				\
		.timing2 = 0x00001042,				\
	},							\
}

#define NAND_MAX_CS	2

#elif defined(CONFIG_NS_PLUS)

#define REG_NAND_BASE				0x18026000
#define REG_NAND_IDM_BASE			0x1811b000
#define REG_NAND_STRAPS_BASE			0x1803f2a0

#define NAND_STRAP_TYPE_MASK			0x0000f000
#define NAND_STRAP_TYPE_SHIFT			12
#define NAND_STRAP_PAGE_MASK			0x00000c00
#define NAND_STRAP_PAGE_SHIFT			10
/* No bus width strap */
#define NAND_STRAP_WIDTH_MASK			0x0
#define NAND_STRAP_WIDTH_SHIFT			0

#define NAND_STRAP_TYPE_DATA			\
/* sector_1k, ecclevel, spare_size */		\
{						\
	{ 0,  0, 16 },				\
	{ 0, 15, 16 }, /* Hamming ECC */	\
	{ 0,  4, 16 },				\
	{ 0,  8, 16 },				\
	{ 0,  8, 27 },				\
	{ 0, 12, 27 },				\
	{ 1, 12, 27 },				\
	{ 1, 15, 27 },				\
	{ 1, 20, 45 }				\
}

#define NAND_STRAP_PAGE_DATA			\
{						\
	2048, 2048, 4096, 8192			\
}

/*
 * iProc NAND timing configurations for ONFI timing modes [0-5]
 *
 * Clock tick = 4ns
 * Multiplier:
 * x1: tWP tWH tRP tREH tCLH tALH
 * x4: tCS tADL tWB tWHR
 */
#define NAND_TIMING_DATA					\
{								\
	/* ONFI timing mode 0 :				*/	\
	/* tWC=100ns tWP=50ns tWH=30ns			*/	\
	/* tRC=100ns tRP=50ns tREH=30ns			*/	\
	/* tCS=70ns tCLH=20ns tALH=20ns tADL=200ns	*/	\
	/* tWB=200ns tWHR=120ns tREA=40ns		*/	\
	{							\
		.timing1 = 0xfafa558d,				\
		.timing2 = 0x00001a85,				\
	},							\
	/* ONFI timing mode 1 :				*/	\
	/* tWC=45 tWP=25ns tWH=15ns			*/	\
	/* tRC=50 tRP=25ns tREH=15ns			*/	\
	/* tCS=35ns tCLH=10ns tALH=10ns tADL=100ns	*/	\
	/* tWB=100ns tWHR=80ns tREA=30ns		*/	\
	{							\
		.timing1 = 0x85853347,				\
		.timing2 = 0x00000e64,				\
	},							\
	/* ONFI timing mode 2 :				*/	\
	/* tWC=35ns tWP=17ns tWH=15ns			*/	\
	/* tRC=35ns tRP=17ns tREH=15ns			*/	\
	/* tCS=25ns tCLH=10ns tALH=10ns tADL=100ns	*/	\
	/* tWB=100ns tWHR=80ns tREA=25ns		*/	\
	{							\
		.timing1 = 0x54542347,				\
		.timing2 = 0x00000e63,				\
	},							\
	/* ONFI timing mode 3 :				*/	\
	/* tWC=30ns tWP=15ns tWH=10ns			*/	\
	/* tRC=30ns tRP=15ns tREH=10ns			*/	\
	/* tCS=25ns tCLH=5ns tALH=5ns tADL=100ns	*/	\
	/* tWB=100ns tWHR=60ns tREA=20ns		*/	\
	{							\
		.timing1 = 0x44442237,				\
		.timing2 = 0x00000e43,				\
	},							\
	/* ONFI timing mode 4 :				*/	\
	/* tWC=25ns tWP=12ns tWH=10ns			*/	\
	/* tRC=25ns tRP=12ns tREH=10ns			*/	\
	/* tCS=20ns tCLH=5ns tALH=5ns tADL=70ns		*/	\
	/* tWB=100ns tWHR=60ns tREA=20ns		*/	\
	{							\
		.timing1 = 0x43432235,				\
		.timing2 = 0x00000e42,				\
	},							\
	/* ONFI timing mode 5 :				*/	\
	/* tWC=20ns tWP=10ns tWH=7ns			*/	\
	/* tRC=20ns tRP=10ns tREH=7ns			*/	\
	/* tCS=15ns tCLH=5ns tALH=5ns tADL=70ns		*/	\
	/* tWB=100ns tWHR=60ns tREA=16ns		*/	\
	{							\
		.timing1 = 0x32321225,				\
		.timing2 = 0x00000e42,				\
	},							\
}

#define NAND_MAX_CS	1

#else
#error "Unsupported configuration"
#endif

#endif /* _IPROC_NAND_H_ */
