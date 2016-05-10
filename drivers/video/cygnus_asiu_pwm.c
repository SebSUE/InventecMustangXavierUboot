/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <asm/io.h>
#include "cygnus_asiu_pwm.h"

#define ASIU_TOP_SW_RESET_CTRL              0x180aa020

#define ASIU_PWM_CONTROL                    0x180aa500
#define ASIU_PRESCALE_CONTROL               0x180aa504
#define ASIU_PWM0_PERIOD_COUNT              0x180aa508
#define ASIU_PWM0_DUTY_CYCLE_HIGH           0x180aa50c

#define CRMU_CHIP_IO_PAD_CONTROL            0x0301d0bc

#define CRMU_IOMUX_CONTROL                  0x03024004
#define CRMU_IOMUX_CTRL3                    0x0301d0d4
#define CRMU_IOMUX_CTRL3__CORE_TO_IOMUX_PWM0_SEL_R	0

#define PWM_CONTROL_SMOOTH_SHIFT(chan)		(24 + (chan))
#define PWM_CONTROL_TRIGGER_SHIFT(chan)		(chan)
#define PWM_CONTROL_POLARITY_SHIFT(chan)    (8 + (chan))

#define PRESCALE_SHIFT(chan)                ((chan) << 2)
#define PRESCALE_MASK(chan)                 (0x7 << PRESCALE_SHIFT(chan))

#define PWM_BACKLIGHT_CHAN 0

void cygnus_pwmc_apply_settings(void)
{
	uint32_t reg_val;

	/*
	 * There must be a min 400ns delay between clearing enable and setting
	 * it. Failing to do this may result in no PWM signal.
	 */
	udelay(1);

	/* Set trigger bit and clear smooth bit to apply new settings */
	reg_val = readl(ASIU_PWM_CONTROL);
	reg_val |= 1 << PWM_CONTROL_TRIGGER_SHIFT(PWM_BACKLIGHT_CHAN);
	writel(reg_val, ASIU_PWM_CONTROL);

	/* PWMOUT_ENABLE must be held high for at least 400 ns. */
	udelay(1);
}

/* enable the backlight */
void cygnus_asiu_pwm_bl_enable(void)
{
	uint32_t reg_val;

	/* IOMUX function select for PWM0, the LCD backlight */
	reg_val = readl(CRMU_IOMUX_CTRL3);
	reg_val &= ~(0x0F << CRMU_IOMUX_CTRL3__CORE_TO_IOMUX_PWM0_SEL_R);
	writel(reg_val, CRMU_IOMUX_CTRL3);

	writel(0, CRMU_CHIP_IO_PAD_CONTROL);

	/*
	 * Clear trigger bit but set smooth bit to maintain old
	 * output. The LCD panel wants a normal polarity pwm signal.
	 */
	reg_val = readl(ASIU_PWM_CONTROL);
	reg_val |= 1 << PWM_CONTROL_SMOOTH_SHIFT(PWM_BACKLIGHT_CHAN);
	reg_val &= ~(1 << PWM_CONTROL_TRIGGER_SHIFT(PWM_BACKLIGHT_CHAN));
	reg_val |= 1 << PWM_CONTROL_POLARITY_SHIFT(PWM_BACKLIGHT_CHAN);
	writel(reg_val, ASIU_PWM_CONTROL);

	/* PWM clock prescale control */
	writel(0, ASIU_PRESCALE_CONTROL);

	/* Set the period and duty cycle */
	writel(50, ASIU_PWM0_PERIOD_COUNT);
	writel(25, ASIU_PWM0_DUTY_CYCLE_HIGH);

	cygnus_pwmc_apply_settings();
}

void cygnus_asiu_pwm_bl_disable(void)
{
	uint32_t reg_val = readl(ASIU_PWM_CONTROL);

	/* Set smooth type to 0 and disable */
	reg_val &= ~(1 << PWM_CONTROL_SMOOTH_SHIFT(PWM_BACKLIGHT_CHAN));
	reg_val &= ~(1 << PWM_CONTROL_TRIGGER_SHIFT(PWM_BACKLIGHT_CHAN));
	writel(reg_val, ASIU_PWM_CONTROL);

	/* Simulate a disable by configuring for zero duty */
	writel(0, ASIU_PWM0_DUTY_CYCLE_HIGH);
	writel(0, ASIU_PWM0_PERIOD_COUNT);

	/* Set prescale to 0 for this channel */
	reg_val = readl(ASIU_PRESCALE_CONTROL);
	reg_val &= ~PRESCALE_MASK(PWM_BACKLIGHT_CHAN);
	writel(reg_val, ASIU_PRESCALE_CONTROL);

	cygnus_pwmc_apply_settings();
}
