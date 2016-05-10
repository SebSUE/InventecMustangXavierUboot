/*****************************************************************************
*
* Copyright 2014 Broadcom Corporation.  All rights reserved.
*
* SPDX-License-Identifier:      GPL-2.0+
*
*****************************************************************************/

#ifndef __CONFIGS_NAND_H
#define __CONFIGS_NAND_H

/* NAND Driver */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0
#define CONFIG_BCMCAPRI_NAND

/*
 * TODO: Temporary workaround to keep the Capri NAND driver
 * functioning. The data cache is not being disabled in
 * hardware-mapped memory regions.
 */
#define CONFIG_SYS_DCACHE_OFF

#if defined(CONFIG_NAND_SAMSUNG_K9F2G08U0C)
/* Samsung K9F2G08U0C */
#define CONFIG_NAND_ADDR_CYCLES		5	/* 4 or 5 only */
#define CONFIG_NAND_PAGE_SHIFT		11	/* 2 KiB */
#define CONFIG_NAND_BLOCK_SHIFT		17	/* 128 KiB */
#define CONFIG_NAND_BANK_SHIFT 		28	/* 256 MiB */
#define CONFIG_NAND_SECTOR_SIZE 	512	/* ECC data sector */
#define CONFIG_NAND_ECC_T 		8	/* ECC correction */
#define CONFIG_NAND_OOB_SIZE 		64	/* OOB size */
#define CONFIG_NAND_TIMING_SELECT	2	/* Use timing mode for controller */
#define CONFIG_NAND_TIMING_MODE		1	/* 50 nsec */
#endif

#if defined(CONFIG_NAND_MICRON_29F2G08ABAEA) || defined(CONFIG_NAND_HYNIX_H27U2G8F2CTR) || defined(CONFIG_NAND_SPANSION_S34ML02G100TF100)
/* Non-ONFI configuration for Micron 29F2G08ABAEA, Hynix H27U2G8F2CTR, Spansion S34ML02G100TF100 */
#define CONFIG_NAND_ADDR_CYCLES		5	/* 4 or 5 only */
#define CONFIG_NAND_PAGE_SHIFT		11	/* 2 KiB */
#define CONFIG_NAND_BLOCK_SHIFT		17	/* 128 KiB */
#define CONFIG_NAND_BANK_SHIFT 		28	/* 256 MiB */
#define CONFIG_NAND_SECTOR_SIZE 	512	/* ECC data sector */
#define CONFIG_NAND_ECC_T 		8	/* ECC correction */
#define CONFIG_NAND_OOB_SIZE 		64	/* OOB size */
#define CONFIG_NAND_TIMING_SELECT	2	/* Use timing mode for controller */
#define CONFIG_NAND_TIMING_MODE		4	/* 50 nsec */
#endif

#if 0
/*
 * ONFI configuration for Micron 29F2G08ABAEA, Hynix H27U2G8F2CTR, Spansion S34ML02G100TF100
 * Even with ONFI, certain parameters must be known in order
 * to properly configure the environment regions
 */
#define CONFIG_NAND_ADDR_CYCLES		5	/* 4 or 5 only */
#define CONFIG_NAND_PAGE_SHIFT 		11	/* 2 KiB */
#define CONFIG_NAND_BLOCK_SHIFT 	17	/* 128 KiB */
#define CONFIG_NAND_BANK_SHIFT 		28	/* 2 GiB */
#define CONFIG_NAND_USE_ONFI			/* Get timing/geometry/ecc from device */
#endif

#if 0
/* Hynix H27U8G85DTR */
#define CONFIG_NAND_ADDR_CYCLES		5	/* 4 or 5 only */
#define CONFIG_NAND_PAGE_SHIFT		11	/* 2 KiB */
#define CONFIG_NAND_BLOCK_SHIFT		17	/* 128 KiB */
#define CONFIG_NAND_BANK_SHIFT 		30	/* 1 GiB */
#define CONFIG_NAND_SECTOR_SIZE 	512	/* ECC data sector */
#define CONFIG_NAND_ECC_T 		8	/* ECC correction */
#define CONFIG_NAND_OOB_SIZE 		64	/* OOB size */
#define CONFIG_NAND_TIMING_SELECT	2	/* Use timing mode for controller */
#define CONFIG_NAND_TIMING_MODE		4	/* 25 nsec */
#endif

#if 0
/* Toshiba TC58NVG1S3ETA0 */
#define CONFIG_NAND_ADDR_CYCLES		5	/* 4 or 5 only */
#define CONFIG_NAND_PAGE_SHIFT		11	/* 2 KiB */
#define CONFIG_NAND_BLOCK_SHIFT		17	/* 128 KiB */
#define CONFIG_NAND_BANK_SHIFT 		28	/* 256 MiB */
#define CONFIG_NAND_SECTOR_SIZE 	512	/* ECC data sector */
#define CONFIG_NAND_ECC_T 		8	/* ECC correction */
#define CONFIG_NAND_OOB_SIZE 		64	/* OOB size */
#define CONFIG_NAND_TIMING_SELECT	2	/* Use timing mode for controller */
#define CONFIG_NAND_TIMING_MODE		4	/* 25 nsec */
#endif

#define CONFIG_NAND_CHIPSIZE (1<<CONFIG_NAND_BANK_SHIFT)
#define CONFIG_NAND_BLOCKSIZE (1<<CONFIG_NAND_BLOCK_SHIFT)

#define CONFIG_CMD_NAND

#define CONFIG_CMD_MTDPARTS

#endif /* __CONFIGS_NAND_H */
