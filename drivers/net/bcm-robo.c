/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>

#include <asm/io.h>
#include <errno.h>

#include "bcm-robo.h"
#include "bcm-sf2-eth.h"

#define BIT(x)	(1 << (x))

/* Command and status register of the SRAB */
#define CFG_F_SRA_RST_MASK	BIT(2)

/* Switch interface controls */
#define CFG_F_SW_INIT_DONE_MASK	BIT(6)

#define REG_VERSION_ID	0x40

#define SRAB_BASE_ADDR	0x18007000
/* each core gets 4Kbytes for registers */
#define SRAB_REG_SIZE	0x1000

#define REG_SRAB_CMDSTAT	0x2C
#define REG_SRAB_WDH	0x30
#define REG_SRAB_WDL	0x34
#define REG_SRAB_RDH	0x38
#define REG_SRAB_RDL	0x3C

#define SRAB_READY_MASK	0x1
#define SRAB_WRITE_MASK	0x2

#define SRAB_PAGE_SHIFT	24
#define SRAB_OFFSET_SHIFT	16

#define SRAB_MAX_RETRY	100

#define SWITCH_RX_PAUSE_CAP_OFFSET	9

#define ETHHW_PORT_0	0
#define ETHHW_PORT_1	1

#define PORT_LAN	ETHHW_PORT_0
#define PORT_PC	ETHHW_PORT_1

/* #define BRCM_TAG true */

/* Page numbers */
#define PAGE_CTRL	0x00	/* Control page */
#define PAGE_STATUS	0x01	/* Status page */
#define PAGE_MMR	0x02	/* 5397 Management/Mirroring page */
#define PAGE_MPORT	0x04	/* MPORT page */
#define PAGE_VLAN	0x34	/* VLAN page */
#define PAGE_MIB_PORT0	0x20	/* MIB page */
#define PAGE_MIB_PORT1	0x21	/* MIB page */
#define PAGE_MIB_PORT5	0x25	/* MIB page */
#define PAGE_MIB_PORT8	0x28	/* MIB page */

#define PAGE_MIB_SNAPSHOT	0x71	/* MIB page */
#define PAGE_BPM	0x73	/* BPM page */

#define PAGE_IOCTRL	0xe4	/* IO Port Informantion Control page */

/* Control page registers */
#define REG_CTRL_PORT0	0x00	/* Port 0 traffic control register */
#define REG_CTRL_PORT1	0x01	/* Port 1 traffic control register */
#define REG_CTRL_PORT5	0x05	/* Port 5 traffic control register */
#define REG_CTRL_IMP	0x08	/* IMP port traffic control register */
#define REG_CTRL_MODE	0x0B	/* Switch Mode register */
#define REG_CTRL_MIIPO	0x0E	/* 5325: MII Port Override register */
#define REG_CTRL_PWRDOWN	0x0F	/* 5325: Power Down Mode register */
#define REG_CTRL_SWCTL	0x22	/* Switch control register */
#define REG_CTRL_PPORT	0x24	/* Protected port register */
#define REG_CTRL_P5RGMIICTL	0x2A	/* Port5 RGMII Control register */

#define REG_CTRL_MDIO_DA	0x6f	/* MDIO Direct Access Enable register */

#define REG_MGMT_CFG	0x00	/* Global Management Configuration */
#define REG_BRCM_HDR	0x03	/* BRCM Header Control */
#define REG_DEVICE_ID	0x30	/* 539x Device id: */

/* MPORT page registers */
#define REG_MPORT_CTRL	0x0E	/* mport control */
#define REG_MPORT_ADDR0	0x10	/* Control 0 address register */
#define REG_MPORT_VCTR0	0x18	/* Control 0 vector resigter */
#define NEXT_MPORT_REG_OFFSET	0x10	/* Offset to next MPORT reg */

#define MPORT_E_TYPE_SHIFT	48

/* VLAN page registers */
#define REG_VLAN_CTRL5	0x06	/* VLAN Control 5 register */

/* IO Port Informantion Control page registers*/
#define REG_IOCTRL_PORT5	0x0a	/* Port 5 traffic control register */
#define REG_IOCTRL_PORTIMP	0x10	/* Port 8 traffic control register */

#define PORT_ALL	0x103
#define LLDP_E_TYPE	0x88CC

/* BPM registers*/
#define REG_BPM_SPARE0	0x60

/* IMP control bits */
#define IMP_C_RX_UCST_EN	16
#define IMP_C_RX_MCST_EN	8
#define IMP_C_RX_BCST_EN	4
#define IMP_C_TX_DIS	2
#define IMP_C_RX_DIS	0

/* Convert MAC addr pointer to 64 bits */
#define MACTOU64(mac)	(((uint64_t)((mac)[0]) << 40) + \
	((uint64_t)((mac)[1]) << 32) + \
	((uint64_t)((mac)[2]) << 24) + \
	((uint64_t)((mac)[3]) << 16) + \
	((uint64_t)((mac)[4]) << 8) + \
	((uint64_t)((mac)[5])))

static int srab_interface_reset(struct esw_info *robo)
{
	int i, ret = 0;
	uint32_t val32;

	/* Wait for switch initialization complete */
	for (i = SRAB_MAX_RETRY * 10; i > 0; i--) {
		val32 = readl(robo->srab_base + REG_SRAB_CMDSTAT);
		if ((val32 & CFG_F_SW_INIT_DONE_MASK))
			break;
		udelay(10);
	}

	/* timed out */
	if (!i) {
		printf("srab_interface_reset: timeout sw_init_done\n");
		ret = -1;
	}

	/* Set the SRAU reset bit */
	writel(CFG_F_SRA_RST_MASK, robo->srab_base + REG_SRAB_CMDSTAT);

	/* Wait for it to auto-clear */
	for (i = SRAB_MAX_RETRY * 10; i > 0; i--) {
		val32 = readl(robo->srab_base + REG_SRAB_CMDSTAT);
		if ((val32 & CFG_F_SRA_RST_MASK) == 0)
			break;
		udelay(10);
	}

	/* timed out */
	if (!i) {
		printf("srab_interface_reset: timeout sra_rst\n");
		ret |= -2;
	}

	return ret;
}

static int check_srab_ready(void *esw)
{
	struct esw_info *robo = (struct esw_info *)esw;
	uint32_t result;
	int i;

	/* Wait for command complete */
	for (i = SRAB_MAX_RETRY; i > 0; i--) {
		result = readl(robo->srab_base + REG_SRAB_CMDSTAT);
		if ((result & SRAB_READY_MASK) == 0)
			break;
		udelay(1);
	}

	/* timed out */
	if (i == 0) {
		printf("srab_request: timeout\n");
		/* TODO srab_interface_reset(robo); */
		return -1;
	}

	return 0;
}

static int esw_read_reg(void *esw, uint8_t page, uint8_t offset,
			void *val, int len)
{
	struct esw_info *robo = (struct esw_info *)esw;
	uint32_t cmd = 0;
	uint32_t data[2];

	uint32_t *p = (uint32_t *)val;

	if (check_srab_ready(esw))
		return -EBUSY;

	/* Issue the read command */
	cmd = ((page << SRAB_PAGE_SHIFT)
	       | (offset << SRAB_OFFSET_SHIFT)
	       | SRAB_READY_MASK);
	writel(cmd, robo->srab_base + REG_SRAB_CMDSTAT);

	/* Wait for command complete */
	if (check_srab_ready(esw))
		return -EBUSY;

	data[0] = readl(robo->srab_base + REG_SRAB_RDL);

	data[1] = readl(robo->srab_base + REG_SRAB_RDH);

	switch (len) {
	case 64:
		p[0] = data[0];
		p[1] = data[1];
		break;
	case 32:
		p[0] = data[0];
		break;
	case 16:
		data[0] &= 0xFFFF;
		*((uint16_t *)p) = (uint16_t)data[0];
		break;
	case 8:
		data[0] &= 0xFF;
		*((uint8_t *)p) = (uint8_t)data[0];
		break;
	default:
		printf("ESW register read error (wrong length %d bits)\n",
		       len);
		return -EINVAL;
	}

	return 0;
}

static int esw_write_reg(void *esw, uint8_t page, uint8_t offset,
			 void *val, int len)
{
	struct esw_info *robo = (struct esw_info *)esw;
	uint32_t cmd = 0;
	uint32_t *p;
	uint32_t data_high = 0;
	uint32_t data_low = *p;

	switch (len) {
	case 64:
		p = (uint32_t *)val;
		data_high = *(p + 1);
		data_low = *p;
		break;
	case 32:
		data_low = *((uint32_t *)val);
		break;
	case 16:
		data_low = *((uint16_t *)val);
		break;
	case 8:
		data_low = *((uint8_t *)val);
		break;
	default:
		printf("ESW register write error (wrong length %d bits)\n",
		       len);
		return -EINVAL;
	}

	/* TODO spinlock to block other ESW reg changes */

	if (check_srab_ready(esw))
		return -EBUSY;

	writel(data_high, robo->srab_base + REG_SRAB_WDH);
	writel(data_low, robo->srab_base + REG_SRAB_WDL);

	/* Issue the write command */
	cmd = ((page << SRAB_PAGE_SHIFT)
	       | (offset << SRAB_OFFSET_SHIFT)
	       | SRAB_READY_MASK | SRAB_WRITE_MASK);
	writel(cmd, robo->srab_base + REG_SRAB_CMDSTAT);

	/* Wait for command complete */
	if (check_srab_ready(esw))
		return -EBUSY;

	return 0;
}

/* SRAB interface functions */
static struct esw_ops srab = {
	esw_read_reg,
	esw_write_reg,
};

/* High level switch configuration functions. */

/* Release access to the RoboSwitch */
static void bcm_robo_detach(struct esw_info *robo)
{
}

/* Get access to the RoboSwitch */
static int bcm_robo_attach(struct esw_info *robo)
{
	int rc;

	robo->srab_base = (void *)SRAB_BASE_ADDR;

	robo->ops = &srab;

	srab_interface_reset(robo);
	esw_read_reg(robo, PAGE_MMR, REG_VERSION_ID, &robo->corerev, 8);

	rc = esw_read_reg(robo, PAGE_MMR, REG_DEVICE_ID, &robo->devid32, 32);
	printf("devid read %ssuccesfully via srab: 0x%x\n",
	       rc ? "un" : "", robo->devid32);

	if ((rc != 0) || (robo->devid32 == 0)) {
		printf("error reading devid\n");
		goto error;
	}
	printf("devid32: 0x%x\n", robo->devid32);

	return 0;

error:
	bcm_robo_detach(robo);
	return -1;
}

static void bcm_robo_pause_frame_init(struct esw_info *robo)
{
	uint32_t reg_val;

	robo->ops->read_reg(robo, PAGE_IOCTRL, 0x18, &reg_val, 32);

	reg_val |= 0x800000; /* Enable override */

	/* Disable PAUSE frames for LAN ports */
	reg_val &= ~(1 << PORT_LAN); /* TX */
	reg_val &= ~(1 << (PORT_LAN + SWITCH_RX_PAUSE_CAP_OFFSET));

	/* Disable PAUSE frames for PC ports too */
	reg_val &= ~(1 << PORT_PC); /* TX */
	reg_val &= ~(1 << (PORT_PC + SWITCH_RX_PAUSE_CAP_OFFSET));

	robo->ops->write_reg(robo, PAGE_IOCTRL, 0x18, &reg_val, 32);
}

/* Enable switching/forwarding */
static int bcm_robo_enable_switch(void *privp)
{
	struct eth_info *eth = (struct eth_info *)privp;
	struct esw_info *robo = &eth->esw;
	int ret = 0;
	uint8_t val8;
	uint16_t val16;

	/* Setup PAUSE capability of ports */
	bcm_robo_pause_frame_init(robo);

	/* Switch Mode register (Page 0, Address 0x0B) */
	robo->ops->read_reg(robo, PAGE_CTRL, REG_CTRL_MODE, &val8, 8);

	/* Set managed mode */
	val8 |= 1;
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_MODE, &val8, 8);

	/* Enable forwarding */
	val8 |= (1 << 1);
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_MODE, &val8, 8);

	/* Read back */
	robo->ops->read_reg(robo, PAGE_CTRL, REG_CTRL_MODE, &val8, 8);
	if (!(val8 & (1 << 1))) {
		printf("robo_enable_switch: enabling forwarding failed\n");
		ret = -1;
	}

	/* No spanning tree for external ports */
	val8 = 0;
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_PORT0, &val8, 8);
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_PORT1, &val8, 8);

	/* make sure external ports are not in protected mode
	 * (Page 0, Address 0x24)
	 */
	val16 = 0;
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_PPORT, &val16, 16);

	/* Over ride IMP(Port8) status to make it link by default */
	robo->ops->read_reg(robo, PAGE_IOCTRL, REG_IOCTRL_PORTIMP, &val16, 16);
	/* 2G_ENABLED:
	 * Page :0x00
	 * ( Offset: 0xe ) IMP Port States Override Register
	 * [6]: GMII SPEED UP 2G
	 */
	val16 |= 0xf1;	/* Make Link pass and override it. */
	robo->ops->write_reg(robo, PAGE_IOCTRL, REG_IOCTRL_PORTIMP, &val16, 16);

	/* IMP(Port8) config BRCM tag */
	val8 = 0;
	robo->ops->read_reg(robo, PAGE_MMR, REG_BRCM_HDR, &val8, 8);

	/* BRCM tag is used to enables special switch forwarding capabilities.
	 * In U-boot, don't support BRCM tag.
	 */
	val8 &= 0xfe;

	robo->ops->write_reg(robo, PAGE_MMR, REG_BRCM_HDR, &val8, 8);

	/* IMP(Port8) Enable receive bcast packets */
	val8 = 0;
	robo->ops->read_reg(robo, PAGE_CTRL, REG_CTRL_IMP, &val8, 8);
	val8 |= 0x4;
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_IMP, &val8, 8);

	/* IMP(Port8) IMP port Enable */
	val8 = 0;
	robo->ops->read_reg(robo, PAGE_MMR, REG_MGMT_CFG, &val8, 8);
	val8 |= 0x80;
	robo->ops->write_reg(robo, PAGE_MMR, REG_MGMT_CFG, &val8, 8);

	return ret;
}

int bcm_esw_init(void *privp)
{
	struct eth_info *eth = (struct eth_info *)privp;
	struct esw_info *robo = &eth->esw;
	uint8_t val8;

	/* Attach to the switch */
	if (bcm_robo_attach(&eth->esw)) {
		printf("Robo switch attach failed\n");
		return -1;
	}

	/* enable ESW direct access to EGPHY */
	/* MDIO Directo Access Enable Register (Page 0x0, Address 00x6f) */
	robo->ops->read_reg(robo, PAGE_CTRL, REG_CTRL_MDIO_DA, &val8, 8);
	val8 |= 1;		/* enable MDIO direct access */
	robo->ops->write_reg(robo, PAGE_CTRL, REG_CTRL_MDIO_DA, &val8, 8);

	/* Enable switching/forwarding */
	if (bcm_robo_enable_switch(privp)) {
		printf("chipattach: robo_enable_switch failed\n");
		return -1;
	}

	return 0;
}

void bcm_esw_write_rst_reason(void *privp, u32 rst_seq)
{
	struct esw_info *robo = &((struct eth_info *)privp)->esw;

	robo->srab_base = (void *)SRAB_BASE_ADDR;

	esw_write_reg(robo, PAGE_BPM, REG_BPM_SPARE0, &rst_seq, 32);
}
