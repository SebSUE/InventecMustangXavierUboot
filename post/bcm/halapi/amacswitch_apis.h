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
#ifndef _AMACSWITCH_APIS_H
#define _AMACSWITCH_APIS_H

#include <asm/io.h>

//#include "../../arch/arm/include/asm/arch-cygnus/socregs.h"
//#include "../include/reg_access.h"

//#include "cygnus_types.h"

//#define AMAC0 0
//#define AMAC1 1
//#define AMAC2 2

//#define NUM_PKTS 32
//#define PKT_SIZE 64

#define ChipcommonG_MII_Management_Control 0x18002000
#define ChipcommonG_MII_Management_Control__MDCDIV_WIDTH 7
#define ChipcommonG_MII_Management_Control__MDCDIV_R 0
#define ChipcommonG_MII_Management_Control__MDCDIV_L 6
#define ChipcommonG_MII_Management_Control__PRE 7
#define ChipcommonG_MII_Management_Control__EXT 9
#define ChipcommonG_MII_Management_Control__BYP 10
#define ChipcommonG_MII_Management_Control__BSY 8
#define ChipcommonG_MII_Management_Command_Data 0x18002004
#define ChipcommonG_MII_Management_Command_Data__TA_R 16
#define ChipcommonG_MII_Management_Command_Data__RA_R 18
#define ChipcommonG_MII_Management_Command_Data__PA_R 23
#define ChipcommonG_MII_Management_Command_Data__OP_R 28
#define ChipcommonG_MII_Management_Command_Data__SB_R 30

#define ChipcommonG_SRAB_RDL 0x1800703c
#define ChipcommonG_SRAB_RDH 0x18007038
#define ChipcommonG_SRAB_CMDSTAT 0x1800702c
#define ChipcommonG_SRAB_WDL 0x18007034
#define ChipcommonG_SRAB_WDH 0x18007030

#define CRMU_IOMUX_CTRL7 0x0301d0e4
#define CRMU_CHIP_IO_PAD_CONTROL 0x0301d0bc
#define CRMU_IOMUX_CTRL7__CORE_TO_IOMUX_CAMERA_SRAM_RGMII_SEL_R 8
#define CRMU_IOMUX_CTRL7__CORE_TO_IOMUX_CAMERA_RGMII_SEL_R 4

#define CDRU_GPHY_PWR_DOWN_CTRL 0x0301d1ac
#define CDRU_RGMII_IO_CONFIG 0x0301d1a8
#define CDRU_RGMII_IO_CONFIG__IOMUX_RGMII_DRV_SEL_R 6

#define AMAC_IDM0_IDM_RESET_CONTROL 0x18110800
#define AMAC_IDM0_IO_CONTROL_DIRECT 0x18110408

#define GMAC0_INTSTATUS 0x18042020
#define GMAC0_INTMASK 0x18024024
#define GMAC0_INTRCVLAZY 0x18042100

#define GMAC0_XMTSTATUS0_0 0x18042210
#define GMAC0_XMTSTATUS1_0 0x18042214
#define GMAC0_XMTCONTROL_0 0x18042200
#define GMAC0_XMTADDR_LOW_0 0x18042208
#define GMAC0_XMTADDR_HIGH_0 0x1804220c
#define GMAC0_XMTPTR_0 0x18042204

#define GMAC0_RCVSTATUS0 0x18042230
#define GMAC0_RCVSTATUS1 0x18042234
#define GMAC0_RCVCONTROL 0x18042220
#define GMAC0_RCVPTR 0x18042224
#define GMAC0_RCVADDR_LOW 0x18042228
#define GMAC0_RCVADDR_HIGH 0x1804222c

#define GMAC0_DEVCONTROL 0x18042000
#define GMAC0_DEVSTATUS 0x18042004

#define GMAC0_RX_ALL_PKTS 0x180423c4
#define GMAC0_RX_GD_OCTETS_LO 0x180423b0
#define GMAC0_RX_GD_OCTETS_HI 0x180423b4
#define GMAC0_RX_GD_PKTS 0x180423b8
#define GMAC0_RX_ALL_OCTETS_LO 0x180423bc
#define GMAC0_RX_ALL_OCTETS_HI 0x180423c0
#define GMAC0_RX_BRDCAST 0x180423c8
#define GMAC0_RX_MULT 0x180423cc
#define GMAC0_RX_64 0x180423d0
#define GMAC0_RX_65_127 0x180423d4
#define GMAC0_RX_128_255 0x180423d8
#define GMAC0_RX_256_511 0x180423dc
#define GMAC0_RX_512_1023 0x180423e0
#define GMAC0_RX_1024_1522 0x180423e4
#define GMAC0_RX_1523_2047 0x180423e8
#define GMAC0_RX_2048_4095 0x180423ec
#define GMAC0_RX_4096_8191 0x180423f0
#define GMAC0_RX_8192_MAX 0x180423f4
#define GMAC0_RX_JAB 0x180423f8
#define GMAC0_RX_OVR 0x180423fc
#define GMAC0_RX_FRAG 0x18042400
#define GMAC0_RX_DROP 0x18042404
#define GMAC0_RX_CRC_ALIGN 0x18042408
#define GMAC0_RX_UND 0x1804240c
#define GMAC0_RX_CRC 0x18042410
#define GMAC0_RX_ALIGN 0x18042414
#define GMAC0_RX_SYM 0x18042418
#define GMAC0_RX_PAUS 0x1804241c
#define GMAC0_RX_CNTRL 0x18042420
#define GMAC0_RX_GD_OCTETS_LO 0x180243b0
#define GMAC0_RX_GD_OCTETS_HI 0x180243b4

#define GMAC0_RXUNICASTPKTS 0x18042428
#define GMAC0_RXSACHANGES 0x18042424
#define GMAC0_RXQCONTROL 0x18042190

#define GMAC0_TX_ALL_PKTS 0x18042314
#define GMAC0_TX_GD_OCTETS_LO 0x18042300
#define GMAC0_TX_GD_OCTETS_HI 0x18042304
#define GMAC0_TX_GD_PKTS 0x18042308
#define GMAC0_TX_ALL_OCTETS_LO 0x1804230c
#define GMAC0_TX_ALL_OCTETS_HI 0x18042310
#define GMAC0_TX_BRDCAST 0x18042318
#define GMAC0_TX_MULT 0x1804231c
#define GMAC0_TX_64 0x18042320
#define GMAC0_TX_65_127 0x18042324
#define GMAC0_TX_128_255 0x18042328
#define GMAC0_TX_256_511 0x1804232c
#define GMAC0_TX_512_1023 0x18042330
#define GMAC0_TX_1024_1522 0x18042334
#define GMAC0_TX_1523_2047 0x18042338
#define GMAC0_TX_2048_4095 0x1804233c
#define GMAC0_TX_4096_8191 0x18042340
#define GMAC0_TX_8192_MAX 0x18042344
#define GMAC0_TX_JAB 0x18042348
#define GMAC0_TX_OVER 0x1804234c
#define GMAC0_TX_FRAG 0x18042350
#define GMAC0_TX_UNDERRUN 0x18042354
#define GMAC0_TX_COL 0x18042358
#define GMAC0_TX_1_COL 0x1804235c
#define GMAC0_TX_M_COL 0x18042360
#define GMAC0_TX_EX_COL 0x18042364
#define GMAC0_TX_LATE 0x18042368
#define GMAC0_TX_DEF 0x1804236c
#define GMAC0_TX_CRS 0x18042370
#define GMAC0_TX_PAUS 0x18042374

#define GMAC0_TXUNICASTPKT 0x18042378
#define GMAC0_TXQCONTROL 0x1804218c

#define SWITCH_FEATURE_CONFIG 0x0301d198
#define SWITCH_GLOBAL_CONFIG 0x0301d194

#define UNIMAC0_COMMAND_CONFIG 0x18042808
#define UNIMAC0_IPG_HD_BKP_CNTL 0x18042804
#define UNIMAC0_FRM_LENGTH 0x18042814
#define UNIMAC0_MAC_0 0x1804280c
#define UNIMAC0_MAC_1 0x18042810
#define UNIMAC0_TAG_0 0x18042848
#define UNIMAC0_TAG_1 0x1804284c
#define UNIMAC0_TX_IPG_LENGTH 0x1804285c

#define RD32(addr) readl(addr)
#define CPU_READ_SINGLE(x, y) readl(x)
#define cpu_rd_single(x, y) readl(x)
#define RdReg(x) readl(x)
#define rd(x) readl(x)
#define WR32(addr, data) writel(data, addr)
#define cpu_wr_single(x, y, z) writel(y, x)
#define wr(x, y) writel(y, x)

typedef enum
{
		TEST_PASS     = 0,
		TEST_FAIL     = 1,
		TEST_ERROR    = 2,
		TEST_NOT_INIT = 3
} test_status_t;

typedef struct {
//Used to pass parameters to the test
   char name[64];
	uint32_t value;

} test_args_struct;

#define MAC_LOOPBACK 10

#define AMAC_PKT_DESC_FLAGS__CRC_APPEND         0
#define AMAC_PKT_DESC_FLAGS__CRC_OVERWRITE      1
#define AMAC_PKT_DESC_FLAGS__CRC_FORWARD        2
#define AMAC_PKT_DESC_FLAGS__CRC_UNDEFINED      3

#define AMAC_PKT_BUFFER_SIZE_IN_BYTES          12292
//#define AMAC_PKT_BUFFER_SIZE_IN_BYTES          1024

#define AMAC_DESC_TABLE_SIZE                    0x2000 //8KB   512-deep
//#define AMAC_DESC_TABLE_SIZE                    0x4
#define AMAC_BUFFER_SPACE_SIZE                  0x600800 //6MB

#define AMAC_TX_PKT_DESCRIPTOR_LENGTH_IN_BYTES  16
#define AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES  16

//#ifdef CYG
//#define AMAC_DESC_TABLE_SIZE                    0x2000 //8KB   512-deep
//#define AMAC_PKT_BUFFER_SIZE_IN_BYTES 256
//#define AMAC_BUFFER_SPACE_SIZE        0x8000
//#endif


//FIXME///
#define GMAC0_XMTSTATUS0_0__XMTSTATE_0 ((64*((31 - 28)+1))+28)
#define GMAC0_XMTSTATUS0_0__CURRDSCR_0 ((64*((12 - 0)+1))+0)
#define GMAC0_XMTSTATUS1_0__ACTIVEDESCR_0 ((64*((12 - 0)+1))+0)
#define GMAC0_XMTSTATUS1_0__XMTERR_0 ((64*((31 - 28)+1))+28)
#define GMAC0_RCVSTATUS0__RCVSTATE ((64*((31 - 28)+1))+28)
#define GMAC0_RCVSTATUS0__CURRDSCR ((64*((12 - 0)+1))+0)
#define GMAC0_RCVSTATUS1__RCVERR ((64*((31 - 28)+1))+28)
#define GMAC0_RCVSTATUS1__ACTIVEDESCR ((64*((12 - 0)+1))+0)


typedef struct {
   uint32_t reserved_0_19       : 20;
   uint32_t flags               : 8;
   uint32_t eot                 : 1;
   uint32_t intOnComp           : 1;
   uint32_t eof                 : 1;
   uint32_t sof                 : 1;

   uint32_t bufCount            : 15;
   uint32_t reserved_47_47      : 1;
   uint32_t addrExt             : 2;
   uint32_t reserved_50_50      : 1;
   uint32_t reserved_51_63      : 13;

   uint32_t dataBufPtrLow;

   uint32_t dataBufPtrHigh;
} amac_pkt_descriptor_t;

typedef struct {
   uint32_t frameLen            : 16;
   uint32_t undefined           : 16;
   uint32_t pktAddrLow          : 32;
   uint32_t pktAddrHigh         : 32;
} amac_tx_pkt_info_t;

typedef struct {
   uint32_t frameLen            : 16;
   uint32_t pktType             : 2;
   uint32_t vlanTagDetected     : 1;
   uint32_t crcError            : 1;
   uint32_t overSize            : 1;
   uint32_t ctfHit              : 1;
   uint32_t ctfError            : 1;
   uint32_t pktOverflow         : 1;
   uint32_t dscrCntr            : 4;
   uint32_t dataType            : 4;
} amac_rx_pkt_info_t;

void amac_init(const uint8_t amac_id,uint32_t bypass_mode);

//This function needs to go into DDR library
void write_pkt_to_memory(uint8_t* packet, uint32_t frame_length, uint32_t start_address);

//AMAC TX DMA engine APIs
void amac_init_tx_dma(const uint8_t amac_id, uint64_t tx_desc_table_base_addr);
uint32_t amac_config_tx_pkt_descriptor(const uint8_t amac_id, uint32_t amac_tx_pkt_desc_addr, uint32_t amac_tx_pkt_desc_tbl_depth, uint64_t pkt_start_address, uint16_t frame_length,uint32_t index);
void amac_get_tx_status(const uint8_t amac_id);
void amac_enable_tx_dma(const uint8_t amac_id, uint32_t amac_tx_pkt_desc_addr_last_entry);
void amac_build_tx_dma_pkt_info(const uint8_t amac_id, uint64_t pkt_addr, uint32_t frame_length);

//AMAC RX DMA engine APIs
void amac_init_rx_dma(const uint8_t amac_id, uint64_t amac_rx_pkt_desc_base, uint16_t amac_rx_pkt_desc_tbl_depth, uint64_t amac_rx_pkt_buffer_base, uint16_t amac_rx_buffer_size_in_bytes);
void amac_get_rx_status(const uint8_t amac_id);
uint32_t amac_update_rcvptr(const uint8_t amac_id,  uint32_t nextDescrOffsetAddr);
amac_pkt_descriptor_t* amac_get_rx_pkt_descriptor_ptr(uint64_t amac_rx_pkt_desc_base, uint32_t rx_descr_offset);
void amac_display_rx_pkt (uint64_t amac_rx_pkt_desc_base, uint32_t rx_descr_offset);

//Entry point into AMAC test suite for console prompt based testing
//void amac_tests(uint8_t* command_string);

//char** parseConfigParams(char* paramValueString, char* delimiter);
//void printmem(uint32_t address, uint32_t num_bytes);
uint32_t cmpmem(uint32_t block1_start_addr, uint32_t block2_start_addr, uint32_t block_size_in_bytes);

void amac_set_config(const uint8_t amac_id, uint32_t loopback, uint32_t cfgValue);

void amac_getrxstats(const uint8_t amac_id);
void amac_gettxstats(const uint8_t amac_id);
void amac_gettxrxstats(const uint8_t amac_id);

//AMAC additional functions
uint32_t unimac_config(const uint8_t amac_id,uint8_t eth_speed,uint8_t rx_pause_ignore,uint8_t hd_ena,uint8_t mac_loop_con,uint8_t ena_ext_config,uint8_t rmt_loop_ena,uint8_t tx_pause_ignore );
void suspend_tx(const uint8_t amac_id);
void disable_tx(const uint8_t amac_id);
void enable_tx(const uint8_t amac_id);
void disable_rx(const uint8_t amac_id);
void enable_rx(const uint8_t amac_id);
uint8_t tx_done(const uint8_t amac_id);
uint8_t rx_done(const uint8_t amac_id);
uint32_t checker_toct2roct(const uint8_t amac_id);
uint32_t checker_tpkt2rpkt(const uint8_t amac_id);
uint32_t checker_t2r(const uint8_t amac_id,uint32_t addr1,uint32_t addr2);
uint32_t checker_zero(const uint8_t amac_id);
uint32_t checker_all(const uint8_t amac_id);
void counter_reset(const uint8_t amac_id);
void waitforTrans(const uint8_t amac_id,uint32_t tx_desc_len,uint32_t rx_desc_len);
uint32_t chkpktdata(const uint8_t amac_id,uint32_t tx_desc_base,uint32_t rx_desc_base,uint32_t chk_noPkt);
uint32_t chkpktdata_add(const uint8_t amac_id,uint32_t tx_desc_base,uint32_t rx_desc_base,uint32_t chk_noPkt);
void waitforTrans_internalloopback(const uint8_t amac_id);



//Unmanaged mode functions
void  cfg_sw(void);
void cfg_sw_0_1(void);
uint32_t  srab_wr(uint32_t page,  uint32_t offset, uint32_t wdatah, uint32_t wdatal);
uint32_t* srab_rd(uint32_t page, uint32_t offset);
uint32_t  setCmdStat(uint32_t page , uint32_t offset , uint32_t rdwr );
uint32_t  wrData(uint32_t datah,  uint32_t datal);
//uint32_t  rdData(uint32_t* rdatah,uint32_t* rdatal);
uint32_t  wait_for_done(void);
uint32_t delay_noise(uint32_t seconds, int noise);
//void switch_bypass_mode(const uint8_t amac_id);
void ccg_mdio_init(void);
void ccg_mdio_read(uint32_t phy_addr, uint32_t reg_addr, uint32_t *rd_data);
void ccg_mdio_write(uint32_t phy_addr, uint32_t reg_addr, uint32_t *wr_data);
void gphy_reg_read(uint32_t phy_addr, uint32_t reg_addr, uint32_t *rd_data);
void gphy_reg_write(uint32_t phy_addr, uint32_t reg_addr, uint32_t *wr_data);
void cfg_sw_gphy0_to_gphy1(void);
void ccg_mdio_init_ext(void);


#endif
