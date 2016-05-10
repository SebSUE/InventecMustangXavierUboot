/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * This is a driver for a single instance of a ARM SP805 watchdog.
 * Specify CONFIG_ARM_SP805_WDT_BASE in your board/chip config to
 * point it to the address of the registers, and also specify
 * the watchdog's frequency using CONFIG_ARM_SP805_WDT_FREQ.
 */

#include <stdbool.h>
#include <common.h>
#include <asm/io.h>

#if !defined CONFIG_ARM_SP805_WDT_BASE || !defined CONFIG_ARM_SP805_WDT_FREQ
#error "Missing config: where is the hardware; what speed?"
#endif

#define U32_MAX				((u32) -1)
#define WDOG_BASE			CONFIG_ARM_SP805_WDT_BASE

#define WDTREG(OFF)			(WDOG_BASE + (OFF))

#define WDT_WDOGLOAD			WDTREG(0x000)
#define WDT_WDOGVALUE			WDTREG(0x004)
#define WDT_WDOGCONTROL			WDTREG(0x008)
#define WDT_WDOGCONTROL_ITEN		0
#define WDT_WDOGCONTROL_RESEN		1
#define WDT_WDOGINTCLR			WDTREG(0x00c)
#define WDT_WDOGRIS			WDTREG(0x010)
#define WDT_WDOGMIS			WDTREG(0x014)
#define WDT_WDOGLOCK			WDTREG(0xc00)

#define WDT_UNLOCK_CODE			0x1ACCE551

/* We divide by two here because the watchdog reset requires
 * two countdown periods.
 */
#define WDT_1MSEC			(CONFIG_ARM_SP805_WDT_FREQ/2/1000)

/* Maximum millsecond delay in watchdog cycles */
#define WDT_MSEC_MAX			(U32_MAX/WDT_1MSEC)

static inline u32 ms_to_cycles(u32 msec)
{
	/* Clamp value at U32_MAX instead of unsigned wrapping */
	return (msec >= WDT_MSEC_MAX) ? U32_MAX : msec * WDT_1MSEC;
}

static inline u32 cycles_to_ms(u32 cycles)
{
	return cycles / WDT_1MSEC;
}

static bool is_watchdog_locked(void)
{
	return readl(WDT_WDOGLOCK) == 1;
}

static bool unlock_watchdog(void)
{
	writel(WDT_UNLOCK_CODE, WDT_WDOGLOCK);
	return !is_watchdog_locked();
}

static bool lock_watchdog(void)
{
	writel(0, WDT_WDOGLOCK);
	return is_watchdog_locked();
}

static void set_watchdog_reload_value(u32 counter)
{
	writel(counter, WDT_WDOGLOAD);
}

static u32 get_watchdog_reload_value(void)
{
	return readl(WDT_WDOGLOAD);
}

static void clear_watchdog_interrupt(void)
{
	writel(1, WDT_WDOGINTCLR);
}

static u32 get_watchdog_current_value(void)
{
	return readl(WDT_WDOGVALUE);
}

static void arm_watchdog(void)
{
	const u32 enable_mask = (1ul << WDT_WDOGCONTROL_ITEN) |
				(1ul << WDT_WDOGCONTROL_RESEN);
	writel(enable_mask, WDT_WDOGCONTROL);
}

static void disarm_watchdog(void)
{
	const u32 disable_mask = (0ul << WDT_WDOGCONTROL_ITEN) |
				 (0ul << WDT_WDOGCONTROL_RESEN);
	writel(disable_mask, WDT_WDOGCONTROL);
}

static bool is_watchdog_armed(void)
{
	const u32 enable_mask = (1ul << WDT_WDOGCONTROL_ITEN) |
				(1ul << WDT_WDOGCONTROL_RESEN);
	return (readl(WDT_WDOGCONTROL) & enable_mask) != 0;
}

/*
 * Change the load value of the watchdog to msec.
 * If start is true, then start the watchdog also.
 */
static bool configure_watchdog(u32 reload_value, bool start)
{
	bool success = false;

	if (!unlock_watchdog()) {
		printf("%s: failed to unlock watchdog\n", __func__);
		goto out;
	}

	if (start)
		disarm_watchdog();

	clear_watchdog_interrupt();
	set_watchdog_reload_value(reload_value);

	if (get_watchdog_reload_value() != reload_value) {
		/* This is written this way because checkpatch.pl doesn't allow
		 * lines longer than 80 characters, AND it also doesn't allow
		 * string literals expressed as mulstiple adjacent string
		 * literals.
		 */
		printf("%s: watchdog_configure: %s %s", __func__,
		       "failed to update", "watchdog reload value");
		goto lock_out;
	}

	if (start)
		arm_watchdog();

lock_out:
	if (!lock_watchdog()) {
		printf("%s: failed to lock watchdog\n", __func__);
		goto out;
	}

	success = true;

out:
	return success;
}

#ifdef CONFIG_TARGET_BCMCYGNUS
bool stop_watchdog(void)
#else
static bool stop_watchdog(void)
#endif	
{
	bool success = false;

	if (!unlock_watchdog()) {
		printf("%s: failed to unlock watchdog\n", __func__);
		goto out;
	}


	disarm_watchdog();
	clear_watchdog_interrupt();

	/* Following logic copied from Boot1: necessary? */
	(void) get_watchdog_current_value();

	if (!lock_watchdog()) {
		printf("%s: failed to lock watchdog\n", __func__);
		goto out;
	}

	success = true;
out:
	return success;
}

static bool kick_watchdog(void)
{
	bool success = false;

	if (!unlock_watchdog()) {
		printf("%s: failed to unlock watchdog\n", __func__);
		goto out;
	}

	disarm_watchdog();
	clear_watchdog_interrupt();
	arm_watchdog();

	if (!lock_watchdog()) {
		printf("%s: failed to lock watchdog\n", __func__);
		goto out;
	}

	success = true;
out:
	return success;
}

void hw_watchdog_reset(void)
{
	if (is_watchdog_armed())
		kick_watchdog();
}
