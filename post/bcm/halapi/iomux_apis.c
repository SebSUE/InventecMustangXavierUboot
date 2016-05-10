#include <common.h>
#include <asm/io.h>
#include <post.h>
#include "iomux_apis.h"

void iomux_select_alt1_chip_gpio106_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000010;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt1_chip_gpio107_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000010;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt1_chip_gpio108_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000010;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt1_chip_gpio109_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000010;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt1_chip_gpio98_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000001;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt1_chip_gpio_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff00;
	wdata_tmp = 0x00000011;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio32_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio33_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio34_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio35_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio36_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio37_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio38_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio39_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio40_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio41_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio42_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio43_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio44_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio45_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio46_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio47_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio48_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio49_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio50_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio51_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio52_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio53_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio54_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio55_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio56_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio57_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio98_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000002;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt_chip_gpio_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000002;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_alt_sw_led1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000002;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_asiu_debug_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0x00000000;
	wdata_tmp = 0x44444444;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0x00000000;
	wdata_tmp = 0x44444444;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0x00000000;
	wdata_tmp = 0x44444444;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffffff00;
	wdata_tmp = 0x00000044;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000004;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_audio_dte_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0x00ffffff;
	wdata_tmp = 0x22000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0x00ffffff;
	wdata_tmp = 0x22000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_camera_led_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_camera_rgmii_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_camera_sram_rgmii_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_can0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000100;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_can1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffff0fff;
	wdata_tmp = 0x00001000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio100_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio101_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio102_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio103_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio104_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio105_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio106_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio107_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio108_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio109_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio110_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio111_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio112_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio113_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio114_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio115_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio116_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio117_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio118_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio119_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio120_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio121_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio122_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio123_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio124_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio125_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xf0ffffff;
	wdata_tmp = 0x03000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio126_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xf0ffffff;
	wdata_tmp = 0x03000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio127_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xf0ffffff;
	wdata_tmp = 0x03000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio128_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio129_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio130_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio131_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio132_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio133_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio134_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio135_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio136_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio137_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio138_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio139_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio140_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio24_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio25_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio26_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_chip_gpio27_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio28_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf0ffffff;
	wdata_tmp = 0x03000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio29_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf0ffffff;
	wdata_tmp = 0x03000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio30_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_chip_gpio31_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf0ffffff;
	wdata_tmp = 0x03000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio32_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio33_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio34_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio35_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio36_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio37_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio38_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio39_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio40_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio41_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio42_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio43_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio44_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio45_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio46_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio47_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio48_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio49_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio50_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio51_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio52_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio53_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio54_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio55_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio56_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio57_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio58_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio59_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio60_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio61_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio62_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio63_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio64_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio65_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio66_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio67_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio68_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio69_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio70_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio71_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffff0fff;
	wdata_tmp = 0x00003000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio72_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffff0fff;
	wdata_tmp = 0x00003000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio73_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffff0fff;
	wdata_tmp = 0x00003000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_chip_gpio74_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio75_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xff0fffff;
	wdata_tmp = 0x00300000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio76_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio77_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio78_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffff0fff;
	wdata_tmp = 0x00003000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio79_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffff0fff;
	wdata_tmp = 0x00003000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_chip_gpio80_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio81_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio82_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio83_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio84_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio85_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00030000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_chip_gpio86_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio87_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio88_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio89_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio90_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio91_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio92_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000030;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio93_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffff0fff;
	wdata_tmp = 0x00003000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_chip_gpio94_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio95_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio96_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio97_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio98_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000003;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio99_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_chip_gpio_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf000ffff;
	wdata_tmp = 0x03330000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0f0000;
	wdata_tmp = 0x00303333;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff000000;
	wdata_tmp = 0x00333333;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xf0000000;
	wdata_tmp = 0x03333333;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xffff0000;
	wdata_tmp = 0x00003333;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xffffff00;
	wdata_tmp = 0x00000033;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_d1w_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_gpio0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio10_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio11_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio12_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio13_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio14_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xf0ffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio15_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0x0fffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio16_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio17_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio18_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio19_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio20_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio21_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio22_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xf0ffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio23_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0x0fffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_gpio2_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio3_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio5_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio6_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xf0ffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio7_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0x0fffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_gpio8_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio9_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
}

void iomux_select_gpio_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0x00000000;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0x00000000;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0x00000000;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_i2s_0_alt_if_en_interface_1(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00100000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_i2s_0_if_en_interface_1(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00100000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000001;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_i2s_1_alt_if_en_interface_1(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf0ffffff;
	wdata_tmp = 0x01000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_i2s_1_if_en_interface_1(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf0ffffff;
	wdata_tmp = 0x01000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000010;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_i2s_2_alt_if_en_interface_1(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xffff0000;
	wdata_tmp = 0x00002222;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_i2s_2_if_en_interface_1(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xfff00000;
	wdata_tmp = 0x00022222;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_keypad4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xffff0000;
	wdata_tmp = 0x00001111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xffff0000;
	wdata_tmp = 0x00001111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_keypad5_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0xfff00000;
	wdata_tmp = 0x00011111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xfff00000;
	wdata_tmp = 0x00011111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_keypad_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0x00000000;
	wdata_tmp = 0x11111111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0x00000000;
	wdata_tmp = 0x11111111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_lcd_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_nand_flash_16b_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0x00000000;
	wdata_tmp = 0x11111111;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_nand_flash_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_otp_prog_mode_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000300;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_pwm0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_pwm1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_pwm2_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_pwm3_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_pwm4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_pwm5_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0xf0ffffff;
	wdata_tmp = 0x02000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
}

void iomux_select_qspi_flash_gpio_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_qspi_flash_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_rgmii_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff00f;
	wdata_tmp = 0x00000220;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_sdio0_cd_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_sdio0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_sdio0_mmc_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_sdio1_can0_spi4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_sdio1_can1_spi4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_sdio1_cd_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_sdio1_led_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_sdio1_mmc_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xf0ffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_smart_card0_fcb_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_smart_card0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_smart_card1_fcb_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(SMART_CARD_FCB_SEL);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, SMART_CARD_FCB_SEL);
	rd_data = readl(SMART_CARD_FCB_SEL);

	post_log("SMART_CARD_FCB_SEL: %x\n ", rd_data);
}

void iomux_select_smart_card1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf0ffffff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_spdif_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xff0fffff;
	wdata_tmp = 0x00100000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
}

void iomux_select_spi0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_spi1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_spi2_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_spi3_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_spi4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xffff00ff;
	wdata_tmp = 0x00002200;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_spi5_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_spi_sw_led0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xffff0fff;
	wdata_tmp = 0x00002000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}

void iomux_select_sram_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xff0fffff;
	wdata_tmp = 0x00100000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000100;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_strap_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0x0fffffff;
	wdata_tmp = 0xa0000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	mask = 0xf00f0000;
	wdata_tmp = 0x0aa0aaaa;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL3);
	rd_data = readl(CRMU_IOMUX_CTRL3);

	post_log("CRMU_IOMUX_CTRL3: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfff00000;
	wdata_tmp = 0x000aaaaa;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xff0f0000;
	wdata_tmp = 0x00a0aaaa;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_sw_led1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL7);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000001;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL7);
	rd_data = readl(CRMU_IOMUX_CTRL7);

	post_log("CRMU_IOMUX_CTRL7: %x\n ", rd_data);
}

void iomux_select_sw_led2_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00020000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	mask = 0xff0fffff;
	wdata_tmp = 0x00200000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL6);
	rd_data = readl(CRMU_IOMUX_CTRL6);

	post_log("CRMU_IOMUX_CTRL6: %x\n ", rd_data);
}

void iomux_select_switch_debug_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL0);

	mask = 0x00000000;
	wdata_tmp = 0x33333333;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL0);
	rd_data = readl(CRMU_IOMUX_CTRL0);

	post_log("CRMU_IOMUX_CTRL0: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	mask = 0x00000000;
	wdata_tmp = 0x33333333;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL1);
	rd_data = readl(CRMU_IOMUX_CTRL1);

	post_log("CRMU_IOMUX_CTRL1: %x\n ", rd_data);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	mask = 0xffffff00;
	wdata_tmp = 0x00000033;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL2);
	rd_data = readl(CRMU_IOMUX_CTRL2);

	post_log("CRMU_IOMUX_CTRL2: %x\n ", rd_data);
}

void iomux_select_uart0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_uart1_dte_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_uart1_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffff0ff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_uart2_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffffff0f;
	wdata_tmp = 0x00000010;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_uart3_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xffff0fff;
	wdata_tmp = 0x00000000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_uart_sw_led0_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL5);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000002;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL5);
	rd_data = readl(CRMU_IOMUX_CTRL5);

	post_log("CRMU_IOMUX_CTRL5: %x\n ", rd_data);
}

void iomux_select_usart4_if_en_interface(void)
{
	uint32_t rdata = 0, wdata, wdata_tmp, mask, rd_data;
	rd_data = readl(CRMU_IOMUX_CTRL4);

	mask = 0xfff0ffff;
	wdata_tmp = 0x00010000;
	wdata = (rdata & mask) | (wdata_tmp);
	writel(wdata, CRMU_IOMUX_CTRL4);
	rd_data = readl(CRMU_IOMUX_CTRL4);

	post_log("CRMU_IOMUX_CTRL4: %x\n ", rd_data);
}
