/*
 * =====================================================================================
 *       Filename:  kbd_apis.h
 *    Description:  C header file
 *        Version:  1.0
 *        Created:  28/feb/2014
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Susanth
 *        Company:  Broadcom
 * =====================================================================================
 */
#ifndef _PWMTAMPER_APIS_H
#define _PWMTAMPER_APIS_H

#include <common.h>

#define ASIU_PWM_CONTROL 0x180aa500
#define ASIU_PWM0_PERIOD_COUNT 0x180aa508
#define ASIU_PWM0_DUTY_CYCLE_HIGH 0x180aa50c
#define ASIU_PWM1_PERIOD_COUNT 0x180aa510
#define ASIU_PWM1_DUTY_CYCLE_HIGH 0x180aa514
#define ASIU_PWM2_PERIOD_COUNT 0x180aa518
#define ASIU_PWM2_DUTY_CYCLE_HIGH 0x180aa51c
#define ASIU_PWM3_PERIOD_COUNT 0x180aa520
#define ASIU_PWM3_DUTY_CYCLE_HIGH 0x180aa524
#define ASIU_PWM4_PERIOD_COUNT 0x180aa528
#define ASIU_PWM4_DUTY_CYCLE_HIGH 0x180aa52c
#define ASIU_PWM5_PERIOD_COUNT 0x180aa530
#define ASIU_PWM5_DUTY_CYCLE_HIGH 0x180aa534
#define ASIU_PRESCALE_CONTROL 0x180aa504
#define ASIU_TOP_SW_RESET_CTRL 0x180aa020
#define ChipcommonG_GP_DATA_IN 0x1800a000
#define CRMU_CHIP_IO_PAD_CONTROL 0x0301d0bc
#define CRMU_IOMUX_CONTROL 0x03024004
#define GP_OUT_EN 0x03024808
#define GP_DATA_OUT 0x03024804

void setPwmCtl(uint32_t data);
void setPeriod(uint32_t sigNo, uint32_t data);
void setHighPeriod(uint32_t sigNo, uint32_t data);
void setPrescale(uint32_t data);
void clear_regs(uint32_t dummy);
void pwminit(void);
void setcrmuiomux(void);
void setgpiodata(uint32_t data);
void setgpioenall(void);
void setgpioen(uint32_t data);
void adddelay(uint32_t data);
void setpwmpadmode(void);

#endif
