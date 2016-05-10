#include "pwmtamper_apis.h"
#include <asm/io.h>
#include <post.h>

void setPwmCtl(uint32_t data)
{
	writel(data, ASIU_PWM_CONTROL);
	post_log("ASIU_PWM_CONTROL value written %08X\n", data);
}

void setPeriod(uint32_t sigNo, uint32_t data)
{
	switch (sigNo) {
	case 0:
		writel(data, ASIU_PWM0_PERIOD_COUNT);
		post_log("ASIU_PWM0_PERIOD_COUNT value written %08X\n", data);
		break;
	case 1:
		writel(data, ASIU_PWM1_PERIOD_COUNT);
		post_log("ASIU_PWM1_PERIOD_COUNT value written %08X\n", data);
		break;
	case 2:
		writel(data, ASIU_PWM2_PERIOD_COUNT);
		post_log("ASIU_PWM2_PERIOD_COUNT value written %08X\n", data);
		break;
	case 3:
		writel(data, ASIU_PWM3_PERIOD_COUNT);
		post_log("ASIU_PWM3_PERIOD_COUNT value written %08X\n", data);
		break;
	case 4:
		writel(data, ASIU_PWM4_PERIOD_COUNT);
		post_log("ASIU_PWM4_PERIOD_COUNT value written %08X\n", data);
		break;
	case 5:
		writel(data, ASIU_PWM5_PERIOD_COUNT);
		post_log("ASIU_PWM5_PERIOD_COUNT value written %08X\n", data);
		break;
	}
}

void setHighPeriod(uint32_t sigNo, uint32_t data)
{
	switch (sigNo) {
	case 0:
		writel(data, ASIU_PWM0_DUTY_CYCLE_HIGH);
		post_log("ASIU_PWM0_DUTY_CYCLE_HIGH value written %08X\n", data);
		break;

	case 1:
		writel(data, ASIU_PWM1_DUTY_CYCLE_HIGH);
		post_log("ASIU_PWM1_DUTY_CYCLE_HIGH value written %08X\n", data);
		break;

	case 2:
		writel(data, ASIU_PWM2_DUTY_CYCLE_HIGH);
		post_log("ASIU_PWM2_DUTY_CYCLE_HIGH value written %08X\n", data);
		break;

	case 3:
		writel(data, ASIU_PWM3_DUTY_CYCLE_HIGH);
		post_log("ASIU_PWM3_DUTY_CYCLE_HIGH value written %08X\n", data);
		break;

	case 4:
		writel(data, ASIU_PWM4_DUTY_CYCLE_HIGH);
		post_log("ASIU_PWM4_DUTY_CYCLE_HIGH value written %08X\n", data);
		break;

	case 5:
		writel(data, ASIU_PWM5_DUTY_CYCLE_HIGH);
		post_log("ASIU_PWM5_DUTY_CYCLE_HIGH value written %08X\n", data);
		break;
	}
}

void setPrescale(uint32_t data)
{
	writel(data, ASIU_PRESCALE_CONTROL);
	post_log("ASIU_PRESCALE_CONTROL value written : %08X\n", data);
}

void clear_regs(uint32_t dummy)
{
	writel(0x00000000, (ChipcommonG_GP_DATA_IN + 0x00000028));
}

void pwminit(void)
{
	writel(0, CRMU_CHIP_IO_PAD_CONTROL);
	post_log("ASIU SOFT RESET INITIATED\n");
	/*ASIU SW RESET*/
	writel(0x00000000, ASIU_TOP_SW_RESET_CTRL);
	writel(0x000003ff, ASIU_TOP_SW_RESET_CTRL);
	udelay(1000);
	post_log("ASIU SOFT RESET DONE\n");
}

void setpwmpadmode(void)
{
	writel(0, CRMU_CHIP_IO_PAD_CONTROL);
	post_log("CRMU_CHIP_IO_PAD_CONTROL set to output mode\n");
}

void setgpioen(uint32_t data)
{
	writel(data, GP_OUT_EN);
	post_log("GP_OUT_EN value written %08X\n", data);
}

void setgpioenall(void)
{
	writel(0x3F, GP_OUT_EN);
	post_log("GP_OUT_EN value written 0x3F\n");
}

void setgpiodata(uint32_t data)
{
	writel(data, GP_DATA_OUT);
	post_log("GP_DATA_OUT value written %08X\n", data);
}

void setcrmuiomux(void)
{
	uint32_t data;
	data = readl(CRMU_IOMUX_CONTROL);
	data = data | 0x1;
	writel(data, CRMU_IOMUX_CONTROL);
	post_log("CRMU IOMUX CONFIGURED\n");
}
