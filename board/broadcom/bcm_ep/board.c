/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <config.h>
#include <asm/system.h>
#ifdef CONFIG_IPROC_SDHCI
#include <mmc.h>
#include <sdhci.h>
#include <asm/iproc-common/iproc_sdhci.h>
#endif
#include <broadcom/reset_reason_api.h>
#include <asm/persistent_info.h>

#ifdef CONFIG_RAW_BMP_LOGO
#include <bmp_raw.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

__weak unsigned long get_ramdump_reason(void)
{
	return 0;
}

__weak void clear_ramdump_reason(unsigned long val)
{
	/* Do nothing */
}

__weak int get_reset_reason(void)
{
	return RESET_REASON_UNKNOWN;
}

/*
 * board_init - early hardware init
 */
int board_init(void)
{
	/*
	 * Address of boot parameters passed to kernel
	 * Use default offset 0x100
	 */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	run_command("ramdump", 0);

	return 0;
}

/*
 * dram_init - sets u-boot's idea of sdram size
 */
int dram_init(void)
{
#if defined(CONFIG_RESERVED_SEC_MEM)
	gd->ram_size = get_ram_size((long int *)CONFIG_SYS_SDRAM_BASE,
			(CONFIG_SYS_SDRAM_SIZE - CONFIG_RESERVED_SEC_MEM));
#else
	gd->ram_size = get_ram_size((long int *)CONFIG_SYS_SDRAM_BASE,
				    CONFIG_SYS_SDRAM_SIZE);
#endif
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = gd->ram_size;
}

#ifdef CONFIG_BCM_SF2_ETH
int board_eth_init(bd_t *bis)
{
	int rc = -1;
	printf("Registering BCM sf2 eth\n");
	rc = bcm_sf2_eth_register(0);
	return rc;
}
#endif

#ifdef CONFIG_BCMIPROC_ETH
int board_eth_init(bd_t *bis)
{
	int rc = -1;
	printf("Registering eth\n");
	rc = bcmiproc_eth_register(0);
	return rc;
}
#endif

#ifdef CONFIG_IPROC_SDHCI
int board_mmc_init(bd_t *bis)
{
	int ret;

	/* Register SDIO0 */
	ret = iproc_sdhci_init(0, 0);
	if (ret)
		return ret;

	/* Register SDIO1 */
	ret = iproc_sdhci_init(1, 0);
	return ret;
}
#endif

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
void smp_set_core_boot_addr(unsigned long addr, int corenr)
{
}

void smp_kick_all_cpus(void)
{
}

void smp_waitloop(unsigned previous_address)
{
}
#endif

#ifdef CONFIG_SPLASH_SCREEN
#ifdef CONFIG_RAW_BMP_LOGO
int splash_screen_prepare(void)
{
	char bmp_location[10];
	snprintf(bmp_location, 8, "%08X", bmp_data);
	printf("bmp %p\n", bmp_data);
	setenv("splashimage", bmp_location);
	setenv("splashpos", "m,m");
	return 0;
}
#endif
#endif

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
	char reason_str[20];

	printf("reset reason: %d\n", get_reset_reason());
	snprintf(reason_str, 20, "reset_reason=%d", get_reset_reason());
	setenv("reset_args", reason_str);

	return 0;
}
#endif
