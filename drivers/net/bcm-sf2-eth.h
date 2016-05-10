/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BCM_SF2_ETH_H_
#define _BCM_SF2_ETH_H_

#include <phy.h>
#include "bcm-robo.h"

#define RX_BUF_SIZE	2048
/* RX_BUF_NUM must be power of 2 */
#define RX_BUF_NUM	32

#define TX_BUF_SIZE	2048
/* TX_BUF_NUM must be power of 2 */
#define TX_BUF_NUM	2

/* Support 2 Ethernet ports now */
#define BCM_ETH_MAX_PORT_NUM	2

#define CONFIG_BCM_SF2_ETH_DEFAULT_PORT	0

#define AMAC_WARM_RESET_SEQ 0x1111
#define AMAC_COLD_RESET_SEQ 0x0

/* Maximum times to poll on all Rx Buffers.
 *
 * If in UDP mode (like NFS), burst packets may lead to Rx buffer overlap,
 * so try to process as many packets as possible.
 *
 * However, it has an impact to throughput as well.
 */
#define RX_MAX_RETRY       10

#define RX_CHECK_NEXT_BUF      -2
#define RX_ALL_BUFS_CHECKED    -1

enum {
	MAC_DMA_TX = 1,
	MAC_DMA_RX = 2
};

struct eth_dma {
	void *tx_desc_aligned;
	void *rx_desc_aligned;
	void *tx_desc;
	void *rx_desc;

#ifdef CAPRI
	void *tx_cfg_desc;
	void *tx_cfg_buf;
#endif

	uint8_t *tx_buf;
	uint8_t *rx_buf;

	int cur_tx_index;
	int cur_rx_index;

	int (*tx_packet)(struct eth_dma *dma, void *packet, int length);
	bool (*check_tx_done)(struct eth_dma *dma);

	int (*check_rx_done)(struct eth_dma *dma, uint8_t *buf);

	int (*enable_dma)(struct eth_dma *dma, int dir);
	int (*disable_dma)(struct eth_dma *dma, int dir);
};

struct eth_info {
	struct eth_dma dma;
	phy_interface_t phy_interface;
	struct phy_device *port[BCM_ETH_MAX_PORT_NUM];
	int port_num;

	bool switchmode; /* 1 - switch mode, 0 - switch bypass mode */

	bool cold_reboot;

	/* ESW info */
	struct esw_info esw;

	int (*miiphy_read)(const char *devname, unsigned char phyaddr,
			   unsigned char reg, unsigned short *value);
	int (*miiphy_write)(const char *devname, unsigned char phyaddr,
			    unsigned char reg, unsigned short value);

	int (*mac_init)(struct eth_device *dev);
	int (*enable_mac)(void);
	int (*disable_mac)(void);
	int (*set_mac_addr)(unsigned char *mac);
	int (*set_mac_speed)(int speed, int duplex);
#ifdef CONFIG_BCM_ETH_0_100MB
	int (*set_phy_speed) (struct eth_device *dev, int speed);
#endif
};

#endif /* _BCM_SF2_ETH_H_ */
