/*
 * =====================================================================================
 *
 *       Filename:  swdt_apis.h (secure watchdog timer(CRMU WDOG @ 25MHz))
 *
 *    Description:  C header file
 *
 *        Version:  1.0
 *        Created:  19/Sept/2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Susanth
 *        Company:  Broadcom
 *
 * =====================================================================================
 */
#ifndef _IOMUX_APIS_H
#define _IOMUX_APIS_H

#define CRMU_IOMUX_CTRL0 0x0301d0c8
#define CRMU_IOMUX_CTRL1 0x0301d0cc
#define CRMU_IOMUX_CTRL2 0x0301d0d0
#define CRMU_IOMUX_CTRL3 0x0301d0d4
#define CRMU_IOMUX_CTRL4 0x0301d0d8
#define CRMU_IOMUX_CTRL5 0x0301d0dc
#define CRMU_IOMUX_CTRL6 0x0301d0e0
#define CRMU_IOMUX_CTRL7 0x0301d0e4
#define SMART_CARD_FCB_SEL 0x0301d0e8

void iomux_select_alt1_chip_gpio106_if_en_interface(void);
void iomux_select_alt1_chip_gpio107_if_en_interface(void);
void iomux_select_alt1_chip_gpio108_if_en_interface(void);
void iomux_select_alt1_chip_gpio109_if_en_interface(void);
void iomux_select_alt1_chip_gpio98_if_en_interface(void);
void iomux_select_alt1_chip_gpio_if_en_interface(void);
void iomux_select_alt_chip_gpio32_if_en_interface(void);
void iomux_select_alt_chip_gpio33_if_en_interface(void);
void iomux_select_alt_chip_gpio34_if_en_interface(void);
void iomux_select_alt_chip_gpio35_if_en_interface(void);
void iomux_select_alt_chip_gpio36_if_en_interface(void);
void iomux_select_alt_chip_gpio37_if_en_interface(void);
void iomux_select_alt_chip_gpio38_if_en_interface(void);
void iomux_select_alt_chip_gpio39_if_en_interface(void);
void iomux_select_alt_chip_gpio40_if_en_interface(void);
void iomux_select_alt_chip_gpio41_if_en_interface(void);
void iomux_select_alt_chip_gpio42_if_en_interface(void);
void iomux_select_alt_chip_gpio43_if_en_interface(void);
void iomux_select_alt_chip_gpio44_if_en_interface(void);
void iomux_select_alt_chip_gpio45_if_en_interface(void);
void iomux_select_alt_chip_gpio46_if_en_interface(void);
void iomux_select_alt_chip_gpio47_if_en_interface(void);
void iomux_select_alt_chip_gpio48_if_en_interface(void);
void iomux_select_alt_chip_gpio49_if_en_interface(void);
void iomux_select_alt_chip_gpio50_if_en_interface(void);
void iomux_select_alt_chip_gpio51_if_en_interface(void);
void iomux_select_alt_chip_gpio52_if_en_interface(void);
void iomux_select_alt_chip_gpio53_if_en_interface(void);
void iomux_select_alt_chip_gpio54_if_en_interface(void);
void iomux_select_alt_chip_gpio55_if_en_interface(void);
void iomux_select_alt_chip_gpio56_if_en_interface(void);
void iomux_select_alt_chip_gpio57_if_en_interface(void);
void iomux_select_alt_chip_gpio98_if_en_interface(void);
void iomux_select_alt_chip_gpio_if_en_interface(void);
void iomux_select_alt_sw_led1_if_en_interface(void);
void iomux_select_asiu_debug_if_en_interface(void);
void iomux_select_audio_dte_if_en_interface(void);
void iomux_select_camera_led_if_en_interface(void);
void iomux_select_camera_rgmii_if_en_interface(void);
void iomux_select_camera_sram_rgmii_if_en_interface(void);
void iomux_select_can0_if_en_interface(void);
void iomux_select_can1_if_en_interface(void);
void iomux_select_chip_gpio100_if_en_interface(void);
void iomux_select_chip_gpio101_if_en_interface(void);
void iomux_select_chip_gpio102_if_en_interface(void);
void iomux_select_chip_gpio103_if_en_interface(void);
void iomux_select_chip_gpio104_if_en_interface(void);
void iomux_select_chip_gpio105_if_en_interface(void);
void iomux_select_chip_gpio106_if_en_interface(void);
void iomux_select_chip_gpio107_if_en_interface(void);
void iomux_select_chip_gpio108_if_en_interface(void);
void iomux_select_chip_gpio109_if_en_interface(void);
void iomux_select_chip_gpio110_if_en_interface(void);
void iomux_select_chip_gpio111_if_en_interface(void);
void iomux_select_chip_gpio112_if_en_interface(void);
void iomux_select_chip_gpio113_if_en_interface(void);
void iomux_select_chip_gpio114_if_en_interface(void);
void iomux_select_chip_gpio115_if_en_interface(void);
void iomux_select_chip_gpio116_if_en_interface(void);
void iomux_select_chip_gpio117_if_en_interface(void);
void iomux_select_chip_gpio118_if_en_interface(void);
void iomux_select_chip_gpio119_if_en_interface(void);
void iomux_select_chip_gpio120_if_en_interface(void);
void iomux_select_chip_gpio121_if_en_interface(void);
void iomux_select_chip_gpio122_if_en_interface(void);
void iomux_select_chip_gpio123_if_en_interface(void);
void iomux_select_chip_gpio124_if_en_interface(void);
void iomux_select_chip_gpio125_if_en_interface(void);
void iomux_select_chip_gpio126_if_en_interface(void);
void iomux_select_chip_gpio127_if_en_interface(void);
void iomux_select_chip_gpio128_if_en_interface(void);
void iomux_select_chip_gpio129_if_en_interface(void);
void iomux_select_chip_gpio130_if_en_interface(void);
void iomux_select_chip_gpio131_if_en_interface(void);
void iomux_select_chip_gpio132_if_en_interface(void);
void iomux_select_chip_gpio133_if_en_interface(void);
void iomux_select_chip_gpio134_if_en_interface(void);
void iomux_select_chip_gpio135_if_en_interface(void);
void iomux_select_chip_gpio136_if_en_interface(void);
void iomux_select_chip_gpio137_if_en_interface(void);
void iomux_select_chip_gpio138_if_en_interface(void);
void iomux_select_chip_gpio139_if_en_interface(void);
void iomux_select_chip_gpio140_if_en_interface(void);
void iomux_select_chip_gpio24_if_en_interface(void);
void iomux_select_chip_gpio25_if_en_interface(void);
void iomux_select_chip_gpio26_if_en_interface(void);
void iomux_select_chip_gpio27_if_en_interface(void);
void iomux_select_chip_gpio28_if_en_interface(void);
void iomux_select_chip_gpio29_if_en_interface(void);
void iomux_select_chip_gpio30_if_en_interface(void);
void iomux_select_chip_gpio31_if_en_interface(void);
void iomux_select_chip_gpio32_if_en_interface(void);
void iomux_select_chip_gpio33_if_en_interface(void);
void iomux_select_chip_gpio34_if_en_interface(void);
void iomux_select_chip_gpio35_if_en_interface(void);
void iomux_select_chip_gpio36_if_en_interface(void);
void iomux_select_chip_gpio37_if_en_interface(void);
void iomux_select_chip_gpio38_if_en_interface(void);
void iomux_select_chip_gpio39_if_en_interface(void);
void iomux_select_chip_gpio40_if_en_interface(void);
void iomux_select_chip_gpio41_if_en_interface(void);
void iomux_select_chip_gpio42_if_en_interface(void);
void iomux_select_chip_gpio43_if_en_interface(void);
void iomux_select_chip_gpio44_if_en_interface(void);
void iomux_select_chip_gpio45_if_en_interface(void);
void iomux_select_chip_gpio46_if_en_interface(void);
void iomux_select_chip_gpio47_if_en_interface(void);
void iomux_select_chip_gpio48_if_en_interface(void);
void iomux_select_chip_gpio49_if_en_interface(void);
void iomux_select_chip_gpio50_if_en_interface(void);
void iomux_select_chip_gpio51_if_en_interface(void);
void iomux_select_chip_gpio52_if_en_interface(void);
void iomux_select_chip_gpio53_if_en_interface(void);
void iomux_select_chip_gpio54_if_en_interface(void);
void iomux_select_chip_gpio55_if_en_interface(void);
void iomux_select_chip_gpio56_if_en_interface(void);
void iomux_select_chip_gpio57_if_en_interface(void);
void iomux_select_chip_gpio58_if_en_interface(void);
void iomux_select_chip_gpio59_if_en_interface(void);
void iomux_select_chip_gpio60_if_en_interface(void);
void iomux_select_chip_gpio61_if_en_interface(void);
void iomux_select_chip_gpio62_if_en_interface(void);
void iomux_select_chip_gpio63_if_en_interface(void);
void iomux_select_chip_gpio64_if_en_interface(void);
void iomux_select_chip_gpio65_if_en_interface(void);
void iomux_select_chip_gpio66_if_en_interface(void);
void iomux_select_chip_gpio67_if_en_interface(void);
void iomux_select_chip_gpio68_if_en_interface(void);
void iomux_select_chip_gpio69_if_en_interface(void);
void iomux_select_chip_gpio70_if_en_interface(void);
void iomux_select_chip_gpio71_if_en_interface(void);
void iomux_select_chip_gpio72_if_en_interface(void);
void iomux_select_chip_gpio73_if_en_interface(void);
void iomux_select_chip_gpio74_if_en_interface(void);
void iomux_select_chip_gpio75_if_en_interface(void);
void iomux_select_chip_gpio76_if_en_interface(void);
void iomux_select_chip_gpio77_if_en_interface(void);
void iomux_select_chip_gpio78_if_en_interface(void);
void iomux_select_chip_gpio79_if_en_interface(void);
void iomux_select_chip_gpio80_if_en_interface(void);
void iomux_select_chip_gpio81_if_en_interface(void);
void iomux_select_chip_gpio82_if_en_interface(void);
void iomux_select_chip_gpio83_if_en_interface(void);
void iomux_select_chip_gpio84_if_en_interface(void);
void iomux_select_chip_gpio85_if_en_interface(void);
void iomux_select_chip_gpio86_if_en_interface(void);
void iomux_select_chip_gpio87_if_en_interface(void);
void iomux_select_chip_gpio88_if_en_interface(void);
void iomux_select_chip_gpio89_if_en_interface(void);
void iomux_select_chip_gpio90_if_en_interface(void);
void iomux_select_chip_gpio91_if_en_interface(void);
void iomux_select_chip_gpio92_if_en_interface(void);
void iomux_select_chip_gpio93_if_en_interface(void);
void iomux_select_chip_gpio94_if_en_interface(void);
void iomux_select_chip_gpio95_if_en_interface(void);
void iomux_select_chip_gpio96_if_en_interface(void);
void iomux_select_chip_gpio97_if_en_interface(void);
void iomux_select_chip_gpio98_if_en_interface(void);
void iomux_select_chip_gpio99_if_en_interface(void);
void iomux_select_chip_gpio_if_en_interface(void);
void iomux_select_d1w_if_en_interface(void);
void iomux_select_gpio0_if_en_interface(void);
void iomux_select_gpio10_if_en_interface(void);
void iomux_select_gpio11_if_en_interface(void);
void iomux_select_gpio12_if_en_interface(void);
void iomux_select_gpio13_if_en_interface(void);
void iomux_select_gpio14_if_en_interface(void);
void iomux_select_gpio15_if_en_interface(void);
void iomux_select_gpio16_if_en_interface(void);
void iomux_select_gpio17_if_en_interface(void);
void iomux_select_gpio18_if_en_interface(void);
void iomux_select_gpio19_if_en_interface(void);
void iomux_select_gpio1_if_en_interface(void);
void iomux_select_gpio20_if_en_interface(void);
void iomux_select_gpio21_if_en_interface(void);
void iomux_select_gpio22_if_en_interface(void);
void iomux_select_gpio23_if_en_interface(void);
void iomux_select_gpio2_if_en_interface(void);
void iomux_select_gpio3_if_en_interface(void);
void iomux_select_gpio4_if_en_interface(void);
void iomux_select_gpio5_if_en_interface(void);
void iomux_select_gpio6_if_en_interface(void);
void iomux_select_gpio7_if_en_interface(void);
void iomux_select_gpio8_if_en_interface(void);
void iomux_select_gpio9_if_en_interface(void);
void iomux_select_gpio_if_en_interface(void);
void iomux_select_i2s_0_alt_if_en_interface_1(void);
void iomux_select_i2s_0_if_en_interface_1(void);
void iomux_select_i2s_1_alt_if_en_interface_1(void);
void iomux_select_i2s_1_if_en_interface_1(void);
void iomux_select_i2s_2_alt_if_en_interface_1(void);
void iomux_select_i2s_2_if_en_interface_1(void);
void iomux_select_keypad4_if_en_interface(void);
void iomux_select_keypad5_if_en_interface(void);
void iomux_select_keypad_if_en_interface(void);
void iomux_select_lcd_if_en_interface(void);
void iomux_select_nand_flash_16b_if_en_interface(void);
void iomux_select_nand_flash_if_en_interface(void);
void iomux_select_otp_prog_mode_if_en_interface(void);
void iomux_select_pwm0_if_en_interface(void);
void iomux_select_pwm1_if_en_interface(void);
void iomux_select_pwm2_if_en_interface(void);
void iomux_select_pwm3_if_en_interface(void);
void iomux_select_pwm4_if_en_interface(void);
void iomux_select_pwm5_if_en_interface(void);
void iomux_select_qspi_flash_gpio_if_en_interface(void);
void iomux_select_qspi_flash_if_en_interface(void);
void iomux_select_rgmii_if_en_interface(void);
void iomux_select_sdio0_cd_if_en_interface(void);
void iomux_select_sdio0_if_en_interface(void);
void iomux_select_sdio0_mmc_if_en_interface(void);
void iomux_select_sdio1_can0_spi4_if_en_interface(void);
void iomux_select_sdio1_can1_spi4_if_en_interface(void);
void iomux_select_sdio1_cd_if_en_interface(void);
void iomux_select_sdio1_led_if_en_interface(void);
void iomux_select_sdio1_mmc_if_en_interface(void);
void iomux_select_smart_card0_fcb_if_en_interface(void);
void iomux_select_smart_card0_if_en_interface(void);
void iomux_select_smart_card1_fcb_if_en_interface(void);
void iomux_select_smart_card1_if_en_interface(void);
void iomux_select_spdif_if_en_interface(void);
void iomux_select_spi0_if_en_interface(void);
void iomux_select_spi1_if_en_interface(void);
void iomux_select_spi2_if_en_interface(void);
void iomux_select_spi3_if_en_interface(void);
void iomux_select_spi4_if_en_interface(void);
void iomux_select_spi5_if_en_interface(void);
void iomux_select_spi_sw_led0_if_en_interface(void);
void iomux_select_sram_if_en_interface(void);
void iomux_select_strap_if_en_interface(void);
void iomux_select_sw_led1_if_en_interface(void);
void iomux_select_sw_led2_if_en_interface(void);
void iomux_select_switch_debug_if_en_interface(void);
void iomux_select_uart0_if_en_interface(void);
void iomux_select_uart1_dte_if_en_interface(void);
void iomux_select_uart1_if_en_interface(void);
void iomux_select_uart2_if_en_interface(void);
void iomux_select_uart3_if_en_interface(void);
void iomux_select_uart_sw_led0_if_en_interface(void);
void iomux_select_usart4_if_en_interface(void);

#endif
