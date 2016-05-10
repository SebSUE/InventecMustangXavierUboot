/*****************************************************************************
* Copyright 2006 - 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/
//#define BCM11140_ETH_DEBUG

#ifdef BCM11140_ETH_DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#include <common.h>
#include <malloc.h>
#include <net.h>
#include <config.h>

#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/arch/ethHw.h>
#include <asm/arch/ethHw_dma.h>
#include <asm/gpio.h>

#include <asm/arch/brcm_rdb_esub_clk_mgr_reg.h>
#include <asm/arch/brcm_rdb_chipreg.h>

#include <linux/mii.h>
#include <linux/ethtool.h>

/*
* There could be 2 PHYs conneted.
* In this code,
* The PHY connected to Capri's RGMII port 0 is called PHY0;
* The PHY connected to Capri's RGMII port 1 is called PHY1;
* It could be different from the schematicas labels.
*/
#ifdef CONFIG_BCM11140_FPGA
#define GPIO_PHY_RST0	8
#define GPIO_PHY_RST1	9
#elif defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
#define GPIO_PHY_RST0	49
#define GPIO_PHY_RST1	50
#else
#define GPIO_PHY_RST0	53
#define GPIO_PHY_RST1	55
#endif

#define BCM11140_ETH_DEV_NAME          "bcm11140_eth"
#define ETH_DMA_CONTROLLER     0     // DMA0

#define BCM_NET_MODULE_DESCRIPTION    "Broadcom BCM11140 Ethernet driver"
#define BCM_NET_MODULE_VERSION        "0.1"

#define ETH_DMA_CH_RX         1     // PTM
#define ETH_DMA_CH_TX         0     // MTP

#define ETH_DMA_BURST_SIZE    8
#define ETH_DMA_BLOCK_SIZE    256

/* Memory for RX buffers and RX DMA descriptors. */
#define RX_BUF_SIZE        2048
#define RX_BUF_NUM         8
#define RX_DESC_NUM        RX_BUF_NUM

static uint8_t *rx_tx_buffer, *aligned_rx_tx_buffer;
#define RX_BUF_BASE        (aligned_rx_tx_buffer)
#define RX_DESC_BASE       (RX_BUF_BASE + (RX_BUF_NUM * RX_BUF_SIZE))

#define RX_BUF(i)          (uint8_t *)(RX_BUF_BASE + ((i) * RX_BUF_SIZE))
#define RX_DESC(i)         (ETHHW_DMA_DESC *)(RX_DESC_BASE + ((i) * sizeof( ETHHW_DMA_DESC )))
#define RX_FLUSH_CACHE()

/* Memory for TX buffers and TX DMA descriptors.
 * For every Tx buffer, there must be two descriptors (one for config and one
 * for data) and one config buffer (for config information)
 */
#define TX_BUF_SIZE        2048
#define TX_BUF_NUM         1
#define TX_DESC_NUM        (TX_BUF_NUM * 2)  // Require one config and one data descriptor for every data buffer
#define CFG_BUF_SIZE       8
#define CFG_BUF_NUM        TX_BUF_NUM

/* Starting from RX_BUF_BASE + 128KB for Tx buffer */
#define TX_BUF_BASE        (RX_BUF_BASE + 0x20000)
#define TX_DESC_BASE       (TX_BUF_BASE + (TX_BUF_NUM * TX_BUF_SIZE))
#define CFG_BUF_BASE       (TX_DESC_BASE + (TX_DESC_NUM * sizeof( ETHHW_DMA_DESC )))

#define TX_BUF(i)          (uint8_t *)(TX_BUF_BASE + ((i) * TX_BUF_SIZE))
#define TX_DESC(i)         (ETHHW_DMA_DESC *)(TX_DESC_BASE + ((i) * sizeof( ETHHW_DMA_DESC )))
#define CFG_BUF(i)         (uint8_t *)(CFG_BUF_BASE + ((i) * CFG_BUF_SIZE))

#define TX_FLUSH_CACHE()

#define mdelay(n)	udelay((n)*1000)

static const char banner[] = BCM_NET_MODULE_DESCRIPTION " " BCM_NET_MODULE_VERSION "\n";

#define INVALID_PHYID (-1)

#define BCM54210E_PHYID 0x600d84a0
#define BCM54210E_PHYID_MSK 0xfffffff0

/* BIT 5: RGMII out-of-band status disable
 * 1=RXD contains regular RX data during IPG.
 * 0=RXD contains out-of-band status info in RGMII mode during IPG.
 * default: 1
 */
#define MII_BCM54xx_MISC_CTL_RGMII_OUTBAND_STATUS 0x0020

/*
 * chipreg: periph_misc_reg2: 1:0 rgmii_sel 0x2 RW
 Determines the mode of the RGMII pads.
 Decode is as follows: 00 -> 3.3V GMII. 01 -> 2.5V RGMII. 10 -> 1.5V HSTL Class I.(DEFAULT) 11 -> 1.8V HSTL Class I (non-standard).
*/
typedef enum {
	GMII_3P3 = 0,
	RGMII_2P5,
	HSTL_C1_1P5,
	HSTL_C1_1P8
} rgmii_pad_mode;

static void set_rgmii_pad_mode(rgmii_pad_mode mode)
{
	uint32_t tmp = readl(CHIPREGS_BASE_ADDR + CHIPREG_PERIPH_MISC_REG2_OFFSET);

	tmp &= ~CHIPREG_PERIPH_MISC_REG2_RGMII_SEL_MASK;
	tmp |= ((mode << CHIPREG_PERIPH_MISC_REG2_RGMII_SEL_SHIFT) & CHIPREG_PERIPH_MISC_REG2_RGMII_SEL_MASK);

	writel(tmp, CHIPREGS_BASE_ADDR + CHIPREG_PERIPH_MISC_REG2_OFFSET);
}

#ifdef BCM11140_ETH_DEBUG
static int bcm11140_eth_info( void );

static int bcm11140_eth_info( void )
{
   printf( "\n" );
   printf( "Ethernet Console\n" );
   printf( "================\n" );
   printf( "\n" );
   printf( __FILE__ " built on " __DATE__ " at " __TIME__ "\n" );
   printf( "\n" );
   printf( "Buffer              Address     Num   Size\n" );
   printf( "--------------    ----------   ----   ----\n" );
   printf( "Rx Data           0x%08x   %4i   %4i\n", (int)RX_BUF( 0 ), RX_BUF_NUM, RX_BUF_SIZE );
   printf( "Rx Descriptors    0x%08x   %4i\n", (int)RX_DESC( 0 ), RX_DESC_NUM );
   printf( "Tx Data           0x%08x   %4i   %4i\n", (int)TX_BUF( 0 ), TX_BUF_NUM, TX_BUF_SIZE );
   printf( "Tx Descriptors    0x%08x   %4i\n", (int)TX_DESC( 0 ), TX_DESC_NUM  );
   printf( "Config Data       0x%08x   %4i   %4i\n", (int)CFG_BUF( 0 ), CFG_BUF_NUM, CFG_BUF_SIZE );
   printf( "\n" );
   printf( "DMA Config     Value\n" );
   printf( "----------     -----\n" );
   printf( "Controller      %4i\n", ETH_DMA_CONTROLLER );
   printf( "Rx Channel      %4i\n", ETH_DMA_CH_RX );
   printf( "Tx Channel      %4i\n", ETH_DMA_CH_TX );
   printf( "Burst Size      %4i\n", ETH_DMA_BURST_SIZE );
   printf( "Block Size      %4i\n", ETH_DMA_BLOCK_SIZE );
   printf( "\n" );

   return 0;
}

static void txDump(int index, int len)
{
   uint8_t *bufp;
   int i;

   bufp = (uint8_t *) TX_BUF(index);

   debug("Tx Buf: %d, %d\n", index, len);
   for (i = 0; i < len; i++) {
      debug("%02X ", bufp[i]);
   }
   debug("\n");
}
#endif

#ifdef CONFIG_BCM11140_ETH
static int get_phy_id(int port)
{
	int rc;
	uint32_t msb, lsb;

	rc = ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x2, &msb);
	if (rc != ETHHW_RC_NONE)
		return INVALID_PHYID;
	msb &= 0xffff;

	rc = ethHw_miiGet( port, ETHHW_MII_FLAGS_EXT, 0x3, &lsb);
	if (rc != ETHHW_RC_NONE)
		return INVALID_PHYID;
	lsb &= 0xffff;

	return  (msb << 16) + lsb;
}

#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
static void bcm54210e_10m_rxc(int port)
{
	int rc;
	uint32_t reg32;

	rc = get_phy_id(port);
	if (rc == INVALID_PHYID)
		return;

	if ((rc & BCM54210E_PHYID_MSK) == BCM54210E_PHYID) {
		rc = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x18, 0x7007);
		if (rc != ETHHW_RC_NONE)
			return;

		rc = ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x18, &reg32);
		if (rc != ETHHW_RC_NONE)
			return;

		if (!(reg32 & MII_BCM54xx_MISC_CTL_RGMII_OUTBAND_STATUS)) {
			/* RGMII In-band Control Issue
			* Description(s)
			*	When BCM54210E enables RGMII in-band control and link at 10M speed,
			*	the RXC will be changed to 25 MHz.
			* Workaround
			*	Set expansion register F0h[13:12] = 10.
			*
			* Configure the register for the workaround with these settings:
			* mii_write(phya, 0x18, 0x0c00);
			* mii_write(phya, 0x17, 0x0ff0);
			* mii_write(phya, 0x15, 0x2000);
			* mii_write(phya, 0x18, 0x0400); */
			ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x18, 0x0c00);
			ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x17, 0x0ff0);
			ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x15, 0x2000);
			ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x18, 0x0400);
			printf("set port %d BCM54210E PHY 10Mbps RXC workaround\n", port);
		}
	}
}
#endif
static int ethPhyStart( void )
{
   uint32_t reg32;
   int portspeed;

#ifndef CONFIG_BCM11140_FPGA
   /*** Enable PLL-E ***/

   /* Enable Access to CCU registers */
   writel(
      (1 << ESUB_CLK_MGR_REG_WR_ACCESS_CLKMGR_ACC_SHIFT) |
      (0xA5A5 << ESUB_CLK_MGR_REG_WR_ACCESS_PASSWORD_SHIFT),
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_WR_ACCESS_OFFSET);

   writel(
      readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLLE_POST_RESETB_OFFSET) &
      ~ESUB_CLK_MGR_REG_PLLE_POST_RESETB_I_POST_RESETB_PLLE_MASK,
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLLE_POST_RESETB_OFFSET);

   /* Take PLL out of reset and put into normal mode */
   writel(
      readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLLE_RESETB_OFFSET) |
      ESUB_CLK_MGR_REG_PLLE_RESETB_I_PLL_RESETB_PLLE_MASK,
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLLE_RESETB_OFFSET);

   /* Wait for PLL lock */
   while( !(readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLL_LOCK_OFFSET) & ESUB_CLK_MGR_REG_PLL_LOCK_PLL_LOCK_PLLE_MASK) ) {;}

   writel(
      readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLLE_POST_RESETB_OFFSET) |
      ESUB_CLK_MGR_REG_PLLE_POST_RESETB_I_POST_RESETB_PLLE_MASK,
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_PLLE_POST_RESETB_OFFSET);

   /* Switch esw_sys_clk to use 104MHz(208MHz/2) clock */
   writel(
      (readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET) &
       ~(ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_PLL_SELECT_MASK | ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_DIV_MASK)) |
      (ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_PLL_SELECT_CMD_VAR_208M_CLK << ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_PLL_SELECT_SHIFT) |
      (1 << ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_DIV_SHIFT),
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET);

   writel(
      readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET) |
      ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_TRIGGER_MASK,
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET);

   /* Wait for trigger complete */
   while( (readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET) & ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_TRIGGER_MASK) ) {;}


   /* switch Esub AXI clock to 208MHz */
   writel(
      (readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_OFFSET) &
       ~(ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_PLL_SELECT_MASK |
         ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_PLL_SELECT_OVERRIDE_MASK |
         ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_TRIGGER_MASK)) |
      (ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_PLL_SELECT_CMD_VAR_208M_CLK << ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_PLL_SELECT_SHIFT) |
      (ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_PLL_SELECT_OVERRIDE_MASK),
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_OFFSET);

   writel(
      readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_OFFSET) |
      ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_TRIGGER_MASK,
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_OFFSET);

   /* Wait for trigger complete */
   while( (readl(ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_OFFSET) & ESUB_CLK_MGR_REG_ESUB_AXI_DIV_DEBUG_ESUB_AXI_TRIGGER_MASK) ) {;}

   /* Disable Access to CCU registers */
   writel(
      (0 << ESUB_CLK_MGR_REG_WR_ACCESS_CLKMGR_ACC_SHIFT) |
      (0xA5A5 << ESUB_CLK_MGR_REG_WR_ACCESS_PASSWORD_SHIFT),
      ESUB_CLK_BASE_ADDR + ESUB_CLK_MGR_REG_WR_ACCESS_OFFSET);
#endif

   gpio_request(GPIO_PHY_RST0, "ephy0_rst");
   gpio_request(GPIO_PHY_RST1, "ephy1_rst");

   if ((*(uint32_t *) 0x35001f18) == 0x4000) {
	   /* in dt-blob the reset pin should be set as output high */;
	   puts ("=== warm reset: DO NOT hold PHY in reset! ===\n");
	   gpio_direction_output(GPIO_PHY_RST0, 1);
	   gpio_direction_output(GPIO_PHY_RST1, 1);
   } else {
	   /* Hold both PHYs in reset */
	   gpio_direction_output(GPIO_PHY_RST0, 0);
	   gpio_direction_output(GPIO_PHY_RST1, 0);
	   debug("\nHold both PHYs in RST state for 1ms (> 2us)\n");
	   udelay(10);
   }

#ifdef CONFIG_ETH_PHY_POWER_GPIO
   gpio_request(CONFIG_ETH_PHY_POWER_GPIO, "ephy_power_enable");
   gpio_direction_output(CONFIG_ETH_PHY_POWER_GPIO, 0);
   mdelay(10);
   gpio_set_value( CONFIG_ETH_PHY_POWER_GPIO, 1 );
   mdelay( 100 );
#endif

   /* Take PORT0 PHY out of reset */
   gpio_set_value( GPIO_PHY_RST0, 1 );
   debug("\nbring PHY0 (GPIO_%d) out of RST state\n", GPIO_PHY_RST0);
   debug("\nWait 1ms (> 20us)\n");
   udelay(100);

   /* Set MDIO address */
#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
   ETHHW_REG_SET( ETHHW_REG32( MM_IO_BASE_ESW + 0x00380 ), 1 );
#else
   ETHHW_REG_SET( ETHHW_REG32( MM_IO_BASE_ESW + 0x00380 ), 0 );
#endif

#ifndef CONFIG_BCM11140_FPGA
   /* Disable the RGMII transmit timing delay on the external PHY0 */
   portspeed = ethHw_portSpeed( ETHHW_PORT_0 );
   ethHw_miiSet( 0, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
   ethHw_miiGet( 0, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
   reg32 |= 0x8000; /* write enable */
   if (portspeed == 1000)
   {
      reg32 &= ~0x0200; /* turn off bit 9 to disable RGMII Tx delay */
   }
   else
   {
      reg32 |= 0x0200; /* turn on bit 9 to enable RGMII Tx delay */
   }
   ethHw_miiSet( 0, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
#else
   ethHw_miiGet( 0, ETHHW_MII_FLAGS_EXT, 0x9, &reg32 );
   reg32 &= ~0x00000300;   /* Disable 1000-half/full advertisement */
   ethHw_miiSet( 0, ETHHW_MII_FLAGS_EXT, 0x9, reg32 );
#endif

   /* Enable out of band signaling on the external PHY0 */
   ethHw_miiSet( 0, ETHHW_MII_FLAGS_EXT, 0x18, 0x7007 );
   ethHw_miiGet( 0, ETHHW_MII_FLAGS_EXT, 0x18, &reg32 );
   reg32 |= 0x8000; /* write enable */
   reg32 &= ~MII_BCM54xx_MISC_CTL_RGMII_OUTBAND_STATUS; /* turn off bit 5 for outband signaling */
   ethHw_miiSet( 0, ETHHW_MII_FLAGS_EXT, 0x18, reg32 );
#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
   bcm54210e_10m_rxc(0);
#endif
   /* 
    * Even if the hardware supports only 1 phy, both are available 
    * internally so it doesn't hurt to enable both.
    */
   /* Take PORT1 PHY out of reset */
   gpio_set_value( GPIO_PHY_RST1, 1 );
   debug("\nbring PHY1 (GPIO_%d) out of RST state\n", GPIO_PHY_RST1);
   debug("\nWait 1ms (> 20us)\n");
   mdelay(1);

   /* Set MDIO address */
#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
   ETHHW_REG_SET( ETHHW_REG32( MM_IO_BASE_ESW + 0x00388 ), 0 );
#else
   ETHHW_REG_SET( ETHHW_REG32( MM_IO_BASE_ESW + 0x00388 ), 1 );
#endif

#ifndef CONFIG_BCM11140_FPGA
   /* Disable the RGMII transmit timing delay on the external PHY1 */
   portspeed = ethHw_portSpeed( ETHHW_PORT_1 );
   ethHw_miiSet( 1, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
   ethHw_miiGet( 1, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
   reg32 |= 0x8000; /* write enable */
   if (portspeed == 1000)
   {
      reg32 &= ~0x0200; /* turn off bit 9 to disable RGMII Tx delay */
   }
   else
   {
      reg32 |= 0x0200; /* turn on bit 9 to enable RGMII Tx delay */
   }
   ethHw_miiSet( 1, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
#else
   ethHw_miiGet( 1, ETHHW_MII_FLAGS_EXT, 0x9, &reg32 );
   reg32 &= ~0x00000300;   /* Disable 1000-half/full advertisement */
   ethHw_miiSet( 1, ETHHW_MII_FLAGS_EXT, 0x9, reg32 );
#endif

   /* Enable out of band signaling on the external PHY1 */
   ethHw_miiSet( 1, ETHHW_MII_FLAGS_EXT, 0x18, 0x7007 );
   ethHw_miiGet( 1, ETHHW_MII_FLAGS_EXT, 0x18, &reg32 );
   reg32 |= 0x8000; /* write enable */
   reg32 &= ~MII_BCM54xx_MISC_CTL_RGMII_OUTBAND_STATUS; /* turn off bit 5 for outband signaling */
   ethHw_miiSet( 1, ETHHW_MII_FLAGS_EXT, 0x18, reg32 );
#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
   bcm54210e_10m_rxc(1);
#endif
   return 0;
}
#endif

static int txPacketAdd(int index, size_t len, uint8_t * tx_buf)
{
   uint8_t *bufp;
   ETHHW_DMA_DESC *descp;
   uint64_t *cfgp;

   /* Fill SA and DA */
   memcpy((uint8_t *) TX_BUF(index), tx_buf, 12);
   /* add empty brcmTag */
   memset((uint8_t *) TX_BUF(index) + 12, 0, 4);
   /* Fill other data */
   memcpy((uint8_t *) TX_BUF(index) + 16, tx_buf + 12, len - 12);

   /* Add length of BCM tag */
   len += sizeof(uint32_t);   // Account for addition of Broadcom tag
   
   /* The Ethernet packet has to be >= 64 bytes required by switch 
    * padding it with zeros
    */
   if (len < 64)
   {
      memset((uint8_t *) TX_BUF(index) + len, 0, 64 - len);
      len = 64;
   }

   /* Add 4 bytes for Ethernet FCS/CRC */
   len += 4;

   /* Fill config data 
    * (there is a one-to-one mapping of config data and buffer data) 
    */
   bufp = (uint8_t *) TX_BUF(index);
   cfgp = (uint64_t *) CFG_BUF(index);

   *cfgp = ETHHW_DMA_CFG_EOP_MASK | ETHHW_DMA_CFG_OFFSET( bufp, len );

   /* Setup descriptor */
   bufp = (uint8_t *) TX_BUF(index);
   cfgp = (uint64_t *) CFG_BUF(index);
   descp = (ETHHW_DMA_DESC *) TX_DESC(2 * index);

   // Tx config descriptor
   // The config descriptor allows transfer to be purely 64-bit
   // transactions, spanning an arbitrary number of descriptors,
   // so information must be provided to define offsets and EOP
   ETHHW_DMA_DESC_CREATE( 
      descp,
      cfgp, 
      ETHHW_DMA_MTP_FIFO_ADDR_CFG,
      descp,
      ETHHW_DMA_MTP_CTL_LO, 
      ETHHW_DMA_MTP_TRANSACTION_SIZE( cfgp, sizeof( uint64_t ) ) 
      );

   descp++;

   // Tx data descriptor, only one data buffer will be added
   ETHHW_DMA_DESC_CREATE_NEXT( 
      descp,
      bufp, 
      ETHHW_DMA_MTP_FIFO_ADDR_DATA,
      NULL,
      ETHHW_DMA_MTP_CTL_LO, 
      ETHHW_DMA_MTP_TRANSACTION_SIZE( bufp, len) 
      );

   //Flush data, config, and descriptors to external memory
   TX_FLUSH_CACHE();

#ifdef BCM11140_ETH_DEBUG
   txDump(index, len);
#endif
   return 0;
}

/******************************************************************
 * u-boot net functions
 */
static int bcm11140_eth_send(struct eth_device *dev, volatile void *packet, int length)
{
   uint8_t *buf = (uint8_t *) packet;
   int rc = 0;
   int i = 0;
   
   debug("bcm11140 net start Tx: %d\n", length);

   /* Always use 1st buffer since send is synchronous */
   txPacketAdd(0, length, buf);

   /* Start transmit */
   ethHw_dmaEnable( ETH_DMA_CONTROLLER, ETH_DMA_CH_TX, TX_DESC( 0 ) );

   while (!ETHHW_DMA_MTP_TRANSFER_DONE(TX_DESC(0))) 
   {
      udelay(100);
      debug(".");
      i++;
      if(i > 20)
      {
         error("\nbcm11140 ethernet Tx failure! Already retried 20 times\n");
         rc = -1;
         break;
      }
   }

   ethHw_dmaDisable( ETH_DMA_CONTROLLER, ETH_DMA_CH_TX );

   return rc;
}

static int bcm11140_eth_rcv(struct eth_device *dev)
{
   uint8_t *buf = (uint8_t *) NetRxPackets[0];
   int rc = 0;
   volatile ETHHW_DMA_DESC *descp = RX_DESC( 0 );   // First Rx descriptor
   uint8_t *bufp;
   uint16_t rcvlen;
   uint32_t status;

   //debug("entering bcm11140_eth_rcv\n");

   udelay(50);

   while(1)
   {
      /* Poll Rx queue to get a packet */
      if( ETHHW_DMA_PTM_TRANSFER_DONE( descp ) )
      {
         debug("recieved \n");
         bufp = (uint8_t *)descp->dar;

         status = descp->stat1;   // Get SSTATx
         //rcvlen = ((status >> 13) & 0x0001ffff);
         rcvlen = ETHHW_DMA_BUF_LEN(descp);
         
         if(( rcvlen == 0) || (rcvlen > RX_BUF_SIZE))
         {
            error("Wrong Rx packet size %d, drop it\n", rcvlen);
            ETHHW_DMA_DESC_RX_UPDATE( descp, descp->dar, RX_BUF_SIZE );
            break;
         }

         memcpy(buf, bufp, 12);
         /* skip brcmTag */
         memcpy(buf + 12, bufp + 16, rcvlen - 12 - 4);
         rcvlen -= 4;

#ifdef BCM11140_ETH_DEBUG
         debug("Rx Buf: %d\n", rcvlen);
         int i;
         for (i = 0; i < 40; i++) {
            debug("%02X ", buf[i]);
         }
         debug("\n");
#endif
         /* A packet has been received, so forward to uboot network handler */
         NetReceive(buf, rcvlen);

         ETHHW_DMA_DESC_RX_UPDATE( descp, descp->dar, RX_BUF_SIZE );
      }
      else
      {
         //debug("Rx");
         descp = (ETHHW_DMA_DESC *)(descp)->lli;                   // Advance to next descriptor
         
         if(descp == RX_DESC( 0 ))
         {
            /* Tell caller that no packet was received when Rx queue was polled */
            rc = -1;
            //debug("\nNO Rx\n");
            break;
         }
      }
   }

   return rc;
}

static inline void ethMiiSet(int port, int addr, int data)
{
   //ethHw_miiSet(port, 0, (uint32_t) addr, (uint32_t) data);
}

int bcm11140_miiphy_read(char *devname, unsigned char const addr, 
   unsigned char const reg, unsigned short *const value)
{
   uint32_t RegVal;

   if (ETHHW_RC_NONE == ethHw_miiGet(addr, ETHHW_MII_FLAGS_EXT, (uint32_t) reg, &RegVal))
   {
      *value = (unsigned short)(RegVal & 0xFFFF);
      return 0;
   }

   return -1;
}

int bcm11140_miiphy_write(char *devname, unsigned char const addr, 
   unsigned char const reg, unsigned short const value)
{
   if (ETHHW_RC_NONE == ethHw_miiSet(addr, ETHHW_MII_FLAGS_EXT, (uint32_t) reg, (uint32_t) value))
   {
      return 0;
   }

   return -1;
}

static int bcm11140_eth_write_hwaddr(struct eth_device* dev)
{
   int rc;

   printf("\nMAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
      dev->enetaddr[0], dev->enetaddr[1], dev->enetaddr[2],
      dev->enetaddr[3], dev->enetaddr[4], dev->enetaddr[5]);

   rc = ethHw_arlEntrySet((char *)dev->enetaddr, 0, ETHHW_PORT_INT, 0, 1, 1);

   if (rc != ETHHW_RC_NONE) 
   {
      error("MAC set error!\n");
      return -1;
   }

   return 0;
}

static int bcm11140_eth_open( struct eth_device *dev, bd_t * bt )
{
   uint32_t extPortSpeed;
   ETHHW_DMA_DESC *descp = NULL;
   uint8_t *bufp;
   uint32_t reg32;
   int extPort;
   int i;

   debug( "Enable BCM11140 Ethernet \n" );

   /* Set MAC address from env */
   if (bcm11140_eth_write_hwaddr(dev) != 0) 
   {
      error("MAC set error when opening !\n");
      return -1;
   }

   /* Check which port is connected and take PORT0 with priority */
   if( ethHw_portLinkStatus(ETHHW_PORT_0) )
   {
      extPort = ETHHW_PORT_0;
   }
   else if( ethHw_portLinkStatus(ETHHW_PORT_1) )
   {
      extPort = ETHHW_PORT_1;
   }
   else
   {
      error(" None of Ethernet external ports are connected \n");
      return -1;
   }
   
   extPortSpeed = ethHw_portSpeed(extPort);
   /* Configure internal port speed 
    * always 1Gb (switch->CPU) and follow external port speed (CPU->switch)
    */
#ifdef CONFIG_BCM11140_FPGA
   ethHw_impSpeedSet(100, 100);
#else
   ethHw_impSpeedSet(extPortSpeed, 1000);
#endif

   extPort = ETHHW_PORT_0;
   extPortSpeed = ethHw_portSpeed(extPort);
   debug("external port %d speed: %d\n", extPort, extPortSpeed);
   if (extPortSpeed == 1000)
   {
      /* Disable the RGMII transmit timing delay on the external PHY */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
      ethHw_miiGet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
      reg32 |= 0x8000; /* write enable */
      reg32 &= ~0x0200; /* turn off bit 9 to disable RGMII Tx delay */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
   }
   else
   {
      /* Enable the RGMII transmit timing delay on the external PHY */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
      ethHw_miiGet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
      reg32 |= 0x8000; /* write enable */
      reg32 |= 0x0200; /* turn on bit 9 to enable RGMII Tx delay */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
   }
    
   /* 
    * Even if the hardware supports only 1 phy, both are available 
    * internally so it doesn't hurt to configure both.
    */
   extPort = ETHHW_PORT_1;
   extPortSpeed = ethHw_portSpeed(extPort);
   debug("external port %d speed: %d\n", extPort, extPortSpeed);
   if (extPortSpeed == 1000)
   {
      /* Disable the RGMII transmit timing delay on the external PHY */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
      ethHw_miiGet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
      reg32 |= 0x8000; /* write enable */
      reg32 &= ~0x0200; /* turn off bit 9 to disable RGMII Tx delay */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
   }
   else
   {
      /* Enable the RGMII transmit timing delay on the external PHY */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
      ethHw_miiGet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
      reg32 |= 0x8000; /* write enable */
      reg32 |= 0x0200; /* turn on bit 9 to enable RGMII Tx delay */
      ethHw_miiSet( extPort, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
   }

   /* Enable forwarding to internal port */
   ethHw_impEnableSet(1);
   ethHw_macEnableSet(ETHHW_PORT_INT, 1, 1);

   int rx, tx;
   ethHw_impSpeedGet(&rx, &tx);
   debug("internal port speed: CPU->port(%d), port->CPU(%d)\n",
      tx, rx);

   /* Initialize RX DMA descriptors */
   for (i = 0; i < RX_BUF_NUM; i++) 
   {
      bufp = (uint8_t *) RX_BUF(i);
      descp = (ETHHW_DMA_DESC *) RX_DESC(i);

      // Rx config descriptor
      // The config descriptor allows transfer to be purely 64-bit
      // transactions, spanning an arbitrary number of descriptors,
      // so information must be provided to define offsets and EOP
      ETHHW_DMA_DESC_CREATE( 
         descp,
         ETHHW_DMA_PTM_FIFO_ADDR,
         bufp, 
         descp,
         ETHHW_DMA_PTM_CTL_LO, 
         RX_BUF_SIZE 
         );

      descp++;
   }
   // Wrap last descriptor back to beginning
   descp--;
   ETHHW_DMA_DESC_WRAP(descp, (uint32_t)RX_DESC(0));

   /* Init DMA for Ethernet */
   ethHw_dmaInit(ETH_DMA_CONTROLLER);
   ethHw_dmaDisable(ETH_DMA_CONTROLLER, ETH_DMA_CH_RX);
   ethHw_dmaDisable(ETH_DMA_CONTROLLER, ETH_DMA_CH_TX);

   /* Configure Rx DMA */
   ethHw_dmaRxConfig(ETH_DMA_BURST_SIZE, ETH_DMA_BLOCK_SIZE);
   ethHw_dmaConfig( ETH_DMA_CONTROLLER, ETH_DMA_CH_RX,
              ETHHW_DMA_PTM_CTL_HI, ETHHW_DMA_PTM_CTL_LO,
              ETHHW_DMA_PTM_CFG_HI, ETHHW_DMA_PTM_CFG_LO,
              ETHHW_DMA_PTM_SSTAT, 0 );

   // Configure Tx DMA
   ethHw_dmaTxConfig(ETH_DMA_BURST_SIZE);
   ethHw_dmaConfig( ETH_DMA_CONTROLLER, ETH_DMA_CH_TX,
              ETHHW_DMA_MTP_CTL_HI, ETHHW_DMA_MTP_CTL_LO,
              ETHHW_DMA_MTP_CFG_HI, ETHHW_DMA_MTP_CFG_LO,
              0, ETHHW_DMA_MTP_DSTAT );

   /* If RX DMA is not enabled, packet will be held in switch for a while, 
    * but eventually it will be dropped so that RX DMA should be enabled all the time
    */
   ethHw_dmaEnable(ETH_DMA_CONTROLLER, ETH_DMA_CH_RX, RX_DESC(0));

   debug( "Enable Ethernet Done \n" );

   return 0;
}

static void bcm11140_eth_close(struct eth_device *dev)
{
   debug("entering bcm11140_eth_close!\n");

   ethHw_dmaDisable(ETH_DMA_CONTROLLER, ETH_DMA_CH_RX);
   ethHw_dmaDisable(ETH_DMA_CONTROLLER, ETH_DMA_CH_TX);

   /* Disable forwarding to internal port */
   ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);
   ethHw_impEnableSet(0);

   debug("bcmring_net_close!\n");
}


static int phy_restart_autoneg(int port)
{
	int ctl;
        uint32_t datap;

	ctl = ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, MII_BMCR, &datap);

	if (ctl < 0)
		return ctl;

	datap |= (BMCR_ANENABLE | BMCR_ANRESTART);

	/* Don't isolate the PHY if we're negotiating */
	datap &= ~(BMCR_ISOLATE);

	ctl = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, MII_BMCR, datap);

	return ctl;
}

static int phy_start_autoneg(int port)
{
	int result, ctl;

	/* Disable PHY interrupt */
	ctl = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x10, 0x1000);
	if (ctl < 0)
		return ctl;

	/* PHY DSP Connfig */
	ctl = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x18, 0xc00);
	if (ctl < 0)
		return ctl;
	ctl = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x18, 0x400);
	if (ctl < 0)
		return ctl;

	/* Advertise 10/100BASE-T half/full duplex */
	/* Advertise asymmetric pause */
	/* Advertise full-duplex pause */
	ctl = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x04, 0xde1);
	if (ctl < 0)
		return ctl;

	/* Advertise 1000BASE-T half/full duplex */
	ctl = ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x9, 0x300);
	if (ctl < 0)
		return ctl;

	if ((*(uint32_t *) 0x35001f18) == 0x4000)
		puts ("=== warm reset: DO NOT restart PHY auto-negotiation! ===\n");
	else
		/* restart auto negociation */
		result = phy_restart_autoneg(port);

	return result;
}

#define BYTE_ALIGNMENT 8	/* 8 byte alignment requirement */
#define ALIGN_MASK (BYTE_ALIGNMENT - 1)

int bcm11140_eth_register(u8 dev_num)
{
   struct eth_device *dev;
   int rc;

   dev = (struct eth_device *) malloc(sizeof(struct eth_device));
   if (dev == NULL) {
      return -1;
   }

   printf(banner);

   /* Allocate space for Rx & TX buffers/ */
   if (!(rx_tx_buffer = malloc(SZ_1M+BYTE_ALIGNMENT))) {
       error( "Failed to initialize RC/TX buffers.\n" );
       return -1;
   }

   /* 
    * Note that u-boot malloc returns 8-byte aligned buffers by design
    * so there is no need to perform alignment logic to get 64-bit alignment.
    * But let's do it anyways in case the malloc logic ever changes or this
    * code gets copied to some other bootloader or utility.
    */
   aligned_rx_tx_buffer = (uint8_t *)(((uint32_t)rx_tx_buffer + ALIGN_MASK) & ~ALIGN_MASK);

   memset(dev, 0, sizeof(*dev));
   sprintf(dev->name, "%s-%hu", BCM11140_ETH_DEV_NAME, dev_num);
   
   /* Initialization */
   debug( "Ethernet initialization ..." );

   /*
   * need to set the Capri's RGMII pad mode according to the PHY's operation mode
   */
#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
   set_rgmii_pad_mode(HSTL_C1_1P8);
#endif

#ifdef CONFIG_BCM11140_ETH
   rc = ethPhyStart();
   if( rc == ETHHW_RC_NONE )
   {
      debug( "PHY successful\n" );
   }
   else
   {
      error( "PHY failed\n" );
      return -1;
   }
#endif

   /* Configure internal port speed to 1000Mbps */
   ethHw_impSpeedSet(1000, 1000); /* ( Rx, Tx ) */

   rc = ethHw_Init();
   debug("Ethernet initialization %s (rc=%i)\n",
          ETHHW_RC_SUCCESS(rc) ? "successful" : "failed", rc);

   /* Disable forwarding to internal port (this must be done before
    * forwarding is enabled on the external ports)
    */
   ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);
   /* Disable internal port */
   ethHw_impEnableSet(0);

   /* STP not used so put external ports in forwarding state */
   /* TODO:  If STP support is required, this state control will need 
    * to be moved to the STP application
    */
   ethHw_stpStateSet(ETHHW_PORT_0, ETHHW_STP_STATE_FORWARDING);
   ethHw_stpStateSet(ETHHW_PORT_1, ETHHW_STP_STATE_FORWARDING);

   dev->iobase = 0;

   dev->init = bcm11140_eth_open;
   dev->halt = bcm11140_eth_close;
   dev->send = bcm11140_eth_send;
   dev->recv = bcm11140_eth_rcv;
   dev->write_hwaddr = bcm11140_eth_write_hwaddr;

   eth_register(dev);

#ifdef CONFIG_CMD_MII
   miiphy_register(dev->name, bcm11140_miiphy_read, bcm11140_miiphy_write);
#endif

   phy_start_autoneg(0);
#if defined(CONFIG_BE1_ETH_PHY) || defined(CONFIG_BE2_ETH_PHY) || defined(CONFIG_BE3_ETH_PHY) || defined(CONFIG_BE4_ETH_PHY)
   phy_start_autoneg(1);
#endif

   debug( "Basic ethernet functionality initialized\n" );

#ifdef BCM11140_ETH_DEBUG
   bcm11140_eth_info();
#endif

   return 1;
}
