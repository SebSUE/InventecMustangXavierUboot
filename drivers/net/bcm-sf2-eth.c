/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <net.h>
#include <config.h>

#include <phy.h>
#include <miiphy.h>

#include <asm/io.h>

#include <netdev.h>

#include <broadcom/reset_reason_api.h>
#include <asm/persistent_info.h>

#include "bcm-sf2-eth.h"

#if defined(CONFIG_BCM_SF2_ETH_GMAC)
#include "bcm-sf2-eth-gmac.h"

#elif defined(CONFIG_BCM_SF2_ETH_DMAC)

#include "bcm11130_ethHw.h"
#include "bcm11130_eth_dma.h"
#include "bcm-sf2-eth-dmac.h"
#else
#error "bcm_sf2_eth: NEED to define a MAC!"
#endif

#define BCM_NET_MODULE_DESCRIPTION	"Broadcom Starfighter2 Ethernet driver"
#define BCM_NET_MODULE_VERSION		"0.1"
#define BCM_SF2_ETH_DEV_NAME		"bcm_sf2"

static const char banner[] =
	BCM_NET_MODULE_DESCRIPTION " " BCM_NET_MODULE_VERSION "\n";

static int bcm_sf2_eth_init(struct eth_device *dev)
{
	struct eth_info *eth = (struct eth_info *)(dev->priv);
	struct eth_dma *dma = &(eth->dma);
	struct phy_device *phydev;
	int rc = 0;
	int i;

	rc = eth->mac_init(dev);
	if (rc) {
		error("%s: Couldn't cofigure MAC!\n", __func__);
		return rc;
	}

	/* disable DMA */
	dma->disable_dma(dma, MAC_DMA_RX);
	dma->disable_dma(dma, MAC_DMA_TX);

#ifdef CONFIG_BCM_SF2_ETH_DMAC
	soc_enable_phy();
#endif

	eth->port_num = 0;

	if (eth->cold_reboot) {
		debug("Connecting PHY 0...\n");
		phydev = phy_connect(miiphy_get_dev_by_name(dev->name),
				     0, dev, eth->phy_interface);
	} else {
		/* phy_connect ends up doing a phy_reset. During
		 * warm reboot, we want to avoid this
		 */
		phydev = phy_find_by_mask(
					miiphy_get_dev_by_name(dev->name),
					1 << 0, eth->phy_interface);
	}

	if (phydev != NULL) {
		eth->port[0] = phydev;
		eth->port_num += 1;
	} else {
		debug("No PHY found for port 0\n");
	}

	/* If it's switch mode, need to enable 2nd PHY */
	if (eth->switchmode) {
		if (eth->cold_reboot) {
			debug("Connecting PHY 1...\n");
			phydev = phy_connect(miiphy_get_dev_by_name(dev->name),
						1, dev, eth->phy_interface);
		} else {
			/* phy_connect ends up doing a phy_reset. During
			 * warm reboot, we want to avoid this
			 */
			phydev = phy_find_by_mask(
					miiphy_get_dev_by_name(dev->name),
					1 << 1, eth->phy_interface);
		}

		if (phydev) {
			eth->port[1] = phydev;
			eth->port_num += 1;
		} else {
			debug("No PHY found for port 1\n");
		}
	}

	if (eth->cold_reboot) {
		for (i = 0; i < eth->port_num; i++)
			phy_config(eth->port[i]);
#ifdef CONFIG_BCM_ETH_0_100MB
	eth->set_phy_speed(dev, 100);
#endif
	}

	return rc;
}

/*
 * u-boot net functions
 */

static int bcm_sf2_eth_send(struct eth_device *dev, void *packet, int length)
{
	struct eth_dma *dma = &(((struct eth_info *)(dev->priv))->dma);
	uint8_t *buf = (uint8_t *)packet;
	int rc = 0;
	int i = 0;

	debug("%s enter\n", __func__);

	/* load buf and start transmit */
	rc = dma->tx_packet(dma, buf, length);
	if (rc) {
		debug("ERROR - Tx failed\n");
		return rc;
	}

	while (!(dma->check_tx_done(dma))) {
		udelay(100);
		debug(".");
		i++;
		if (i > 20) {
			error("%s: Tx timeout: retried 20 times\n", __func__);
			rc = -1;
			break;
		}
	}

	debug("%s: Tx rc(0x%x)\n", __func__, rc);
	return rc;
}

static int bcm_sf2_eth_receive(struct eth_device *dev)
{
	struct eth_dma *dma = &(((struct eth_info *)(dev->priv))->dma);
	uint8_t *buf = (uint8_t *)NetRxPackets[0];
	int rcvlen;
	int count = 0;
	int rc = 0;
	int i = 0;

	while (1) {
		/* Poll Rx queue to get a packet */
		rcvlen = dma->check_rx_done(dma, buf);
		if (rcvlen == RX_CHECK_NEXT_BUF) {
			continue;
		} else if (rcvlen == RX_ALL_BUFS_CHECKED) {
			count++;
			if (count > RX_MAX_RETRY) {
				/* No packet received */
				rc = -1;
				/* debug("%s: NO More Rx\n", __func__); */
				break;
			}
		} else if ((rcvlen == 0) || (rcvlen > RX_BUF_SIZE)) {
			error("%s: Wrong Ethernet packet size (%d B), skip!\n",
			      __func__, rcvlen);
			break;
		} else {
			debug("%s: valid packet recieved\n", __func__);

			/* TODO Flush RX buffer DCACHE */
			/* flush_dcache_range((unsigned long)bufp,
			    (unsigned long)(bufp + RX_BUF_SIZE)); */
#ifdef BCM_SF2_ETH_DEBUG
			bufDump(buf, rcvlen);
#endif
			/* Forward received packet to uboot network handler */
			NetReceive(buf, rcvlen);

			if (++i >= PKTBUFSRX)
				i = 0;
			buf = NetRxPackets[i];
		}
	}

	return rc;
}

int bcm_sf2_eth_write_hwaddr(struct eth_device *dev)
{
	int rc = 0;
	struct eth_info *eth = (struct eth_info *)(dev->priv);

	printf(" ETH MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
	       dev->enetaddr[0], dev->enetaddr[1], dev->enetaddr[2],
	       dev->enetaddr[3], dev->enetaddr[4], dev->enetaddr[5]);

#ifdef CONFIG_BCM_SF2_ETH_GMAC
	eth->set_mac_addr(dev->enetaddr);
#else
	rc = ethHw_arlEntrySet((char *)dev->enetaddr, 0, ETHHW_PORT_INT, 0, 1, 1);

	if (rc != ETHHW_RC_NONE) {
		error("MAC set error!\n");
		return -1;
	}
#endif

	return rc;
}

int bcm_sf2_eth_open(struct eth_device *dev, bd_t *bt)
{
	struct eth_info *eth = (struct eth_info *)(dev->priv);
	struct eth_dma *dma = &(eth->dma);
	int i;

	debug("Enabling BCM SF2 Ethernet.\n");

	/* Set MAC address from env */
	if (bcm_sf2_eth_write_hwaddr(dev) != 0) {
		error("%s: MAC set error when opening !\n", __func__);
		return -1;
	}

#ifdef CONFIG_BCM_SF2_ETH_GMAC
	eth->enable_mac();

	/* enable tx and rx DMA */
	dma->enable_dma(dma, MAC_DMA_RX);
	dma->enable_dma(dma, MAC_DMA_TX);

	/*
	 * Need to start PHY here because link speed can change
	 * before each ethernet operation
	 */
	if (phy_startup(eth->port[CONFIG_BCM_SF2_ETH_DEFAULT_PORT]))
		error("%s: PHY %d startup failed!\n",
		      __func__, CONFIG_BCM_SF2_ETH_DEFAULT_PORT);

	/* Set MAC speed using default port */
	if (eth->switchmode) {
		/* If it's switch mode,  must have 2 PHYs enabled,
		 * set internal port to 1G Full duplex always.
		 */
		eth->set_mac_speed(1000, 1);
	} else {
		i = CONFIG_BCM_SF2_ETH_DEFAULT_PORT;
		debug("PHY %d: speed:%d, duplex:%d, link:%d\n", i,
		      eth->port[i]->speed,
		      eth->port[i]->duplex,
		      eth->port[i]->link);
		eth->set_mac_speed(eth->port[i]->speed, eth->port[i]->duplex);
	}
#else
	/* Need to start PHY here because link speed can change
	 * before each ethernet operation
	 */
	for (i = 0; i < eth->port_num; i++) {
		if (phy_startup(eth->port[i])) {
			error("PHY %d startup failed!\n", i);
			if (i == CONFIG_BCM11130_ETH_DEFAULT_PORT) {
				error("No default port %d!\n", i);
				return -1;
			}
		}
	}

	/* Configure internal port speed
	 * Rx: 1Gb (switch->CPU)
	 * Tx: follow external port speed (CPU->switch)
	 */
	ethHw_impSpeedSet(
		phy->port[CONFIG_BCM11130_ETH_DEFAULT_EXT_PORT]->speed, 1000);

	bcm_eth_dma_config(dma);

	/* Enable forwarding to internal port */
	ethHw_impEnableSet(1);
	ethHw_macEnableSet(ETHHW_PORT_INT, 1, 1);

#ifdef BCM_SF2_ETH_DEBUG
	int rx, tx;
	/* ethHw_impSpeedGet(&rx, &tx); */
	debug("internal port speed: CPU->port(%d), port->CPU(%d)\n",
	      tx, rx);
#endif

#endif
	debug("Enable Ethernet Done.\n");

	return 0;
}

void bcm_sf2_eth_close(struct eth_device *dev)
{
	struct eth_info *eth = (struct eth_info *)(dev->priv);
	struct eth_dma *dma = &(eth->dma);

	/* disable DMA */
	dma->disable_dma(dma, MAC_DMA_RX);
	dma->disable_dma(dma, MAC_DMA_TX);

#ifdef CONFIG_BCM_SF2_ETH_GMAC
	eth->disable_mac();

#else
	/* Disable forwarding to internal port */
	ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);
	ethHw_impEnableSet(0);
#endif
}

int bcm_sf2_eth_register(bd_t *bis, u8 dev_num)
{
	struct eth_device *dev;
	struct eth_info *eth;
	int rc;

	dev = (struct eth_device *)malloc(sizeof(struct eth_device));
	if (dev == NULL) {
		error("%s: Not enough memory!\n", __func__);
		return -1;
	}

	eth = (struct eth_info *)malloc(sizeof(struct eth_info));
	if (eth == NULL) {
		error("%s: Not enough memory!\n", __func__);
		return -1;
	}

	printf(banner);

	memset(dev, 0, sizeof(*dev));
	sprintf(dev->name, "%s_%s-%hu", BCM_SF2_ETH_DEV_NAME,
		BCM_SF2_ETH_MAC_NAME, dev_num);

	dev->priv = (void *)eth;
	dev->iobase = 0;

	dev->init = bcm_sf2_eth_open;
	dev->halt = bcm_sf2_eth_close;
	dev->send = bcm_sf2_eth_send;
	dev->recv = bcm_sf2_eth_receive;
	dev->write_hwaddr = bcm_sf2_eth_write_hwaddr;

#ifdef CONFIG_BCM_SF2_ETH_GMAC
	if (gmac_add(dev)) {
		free(eth);
		free(dev);
		error("%s: Adding GMAC failed!\n", __func__);
		return -1;
	}
#else
	if (dmac_add(dev)) {
		free(eth);
		free(dev);
		error("Adding DMAC failed!\n");
		return -1;
	}
#endif

	eth_register(dev);

#ifdef CONFIG_CMD_MII
	miiphy_register(dev->name, eth->miiphy_read, eth->miiphy_write);
#endif

	/* Read reboot reason */
	if (eth->switchmode) {
		if (get_reset_reason() == RESET_REASON_WARM) {
			eth->cold_reboot = false;
			bcm_esw_write_rst_reason((void *)eth,
						 AMAC_WARM_RESET_SEQ);
		} else {
			eth->cold_reboot = true;
			bcm_esw_write_rst_reason((void *)eth,
						 AMAC_COLD_RESET_SEQ);
		}
	} else {
		eth->cold_reboot = true;
	}

	/* Initialization */
	debug("Ethernet initialization ...");

	rc = bcm_sf2_eth_init(dev);
	if (rc != 0) {
		error("%s: configuration failed!\n", __func__);
		/* TODO ask MAC to free memory */
		return -1;
	}

	printf("Basic ethernet functionality initialized\n");

#ifdef BCM_SF2_ETH_DEBUG
	/* TODO bcm11130_eth_info(); */
#endif

	return 0;
}
