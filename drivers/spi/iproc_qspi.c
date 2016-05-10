/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <asm/io.h>
#include <common.h>
#include <malloc.h>
#include <spi.h>
#include "iproc_qspi.h"

#define QSPI_AXI_CLK                        200000000 /* 200MHz */

/* Configurations */
#ifndef CONFIG_IPROC_QSPI_BUS
#error CONFIG_IPROC_QSPI_BUS not defined
#endif                          /* !CONFIG_IPROC_QSPI_BUS */
#ifndef CONFIG_IPROC_QSPI_CS
#error CONFIG_IPROC_QSPI_CS not defined
#endif                          /* !CONFIG_IPROC_QSPI_CS */
#define QSPI_WAIT_TIMEOUT_MS                200U /* msec */

/* Chip attributes */
#define QSPI_REG_BASE                       CONFIG_IPROC_QSPI_BASE_REG
#define CRU_CONTROL_REG                     CONFIG_IPROC_QSPI_CRU_CONTROL_REG
#define SPBR_MIN                            8U
#define SPBR_MAX                            255U
#define NUM_TXRAM                           32U
#define NUM_RXRAM                           32U
#define NUM_CDRAM                           16U

/*
 * Register fields
 */
#define MSPI_SPCR0_MSB_BITS_8               0x00000020
#define BSPI_RAF_CONTROL_START_MASK         0x00000001
#define BSPI_RAF_STATUS_SESSION_BUSY_MASK   0x00000001
#define BSPI_RAF_STATUS_FIFO_EMPTY_MASK     0x00000002
#define BSPI_BITS_PER_PHASE_ADDR_MARK       0x00010000
#define BSPI_BITS_PER_CYCLE_DATA_SHIFT      0
#define BSPI_BITS_PER_CYCLE_ADDR_SHIFT      16

/*
 * Flash opcode and parameters
 */
#define OPCODE_RDSR                         0x05
#define OPCODE_FAST_READ                    0x0B
#define OPCODE_EN4B                         0xB7
#define OPCODE_EX4B                         0xE9
#define OPCODE_BRWR                         0x17

/*
 * Check dual/quad mode configurations
 */
#if (CONFIG_IPROC_BSPI_DATA_LANES >= 1) && (CONFIG_IPROC_BSPI_DATA_LANES <= 4)
#if CONFIG_IPROC_BSPI_DATA_LANES == 3
#error Nubmer of lanes should be 1, 2 or 4.
#endif				/* CONFIG_IPROC_BSPI_DATA_LANES == 3 */
#else				/* 1 <= CONFIG_IPROC_BSPI_DATA_LANES <= 4 */
#error Nubmer of lanes should be 1, 2 or 4.
#endif
#if (CONFIG_IPROC_BSPI_ADDR_LANES >= 1) && (CONFIG_IPROC_BSPI_ADDR_LANES <= 4)
#if CONFIG_IPROC_BSPI_ADDR_LANES == 3
#error Nubmer of lanes should be 1, 2 or 4.
#endif				/* CONFIG_IPROC_BSPI_ADDR_LANES == 3 */
#else				/* 1 <= CONFIG_IPROC_BSPI_ADDR_LANES <= 4 */
#error Nubmer of lanes should be 1, 2 or 4.
#endif

/* MSPI registers */
#define MSPI_SPCR0_LSB_REG			0x000
#define MSPI_SPCR0_MSB_REG			0x004
#define MSPI_SPCR1_LSB_REG			0x008
#define MSPI_SPCR1_MSB_REG			0x00c
#define MSPI_NEWQP_REG				0x010
#define MSPI_ENDQP_REG				0x014
#define MSPI_SPCR2_REG				0x018
#define MSPI_STATUS_REG				0x020
#define MSPI_CPTQP_REG				0x024
#define MSPI_TXRAM_REG				0x040
#define MSPI_RXRAM_REG				0x0c0
#define MSPI_CDRAM_REG				0x140
#define MSPI_WRITE_LOCK_REG			0x180
#define MSPI_DISABLE_FLUSH_GEN_REG		0x184

/* BSPI registers */
#define BSPI_REVISION_ID_REG			0x000
#define BSPI_SCRATCH_REG			0x004
#define BSPI_MAST_N_BOOT_CTRL_REG		0x008
#define BSPI_BUSY_STATUS_REG			0x00c
#define BSPI_INTR_STATUS_REG			0x010
#define BSPI_B0_STATUS_REG			0x014
#define BSPI_B0_CTRL_REG			0x018
#define BSPI_B1_STATUS_REG			0x01c
#define BSPI_B1_CTRL_REG			0x020
#define BSPI_STRAP_OVERRIDE_CTRL_REG		0x024
#define BSPI_FLEX_MODE_ENABLE_REG		0x028
#define BSPI_BITS_PER_CYCLE_REG			0x02C
#define BSPI_BITS_PER_PHASE_REG			0x030
#define BSPI_CMD_AND_MODE_BYTE_REG		0x034
#define BSPI_FLASH_UPPER_ADDR_BYTE_REG		0x038
#define BSPI_XOR_VALUE_REG			0x03C
#define BSPI_XOR_ENABLE_REG			0x040
#define BSPI_PIO_MODE_ENABLE_REG		0x044
#define BSPI_PIO_IODIR_REG			0x048
#define BSPI_PIO_DATA_REG			0x04C

/* RAF registers */
#define BSPI_RAF_START_ADDRESS_REG		0x00
#define BSPI_RAF_NUM_WORDS_REG			0x04
#define BSPI_RAF_CTRL_REG			0x08
#define BSPI_RAF_FULLNESS_REG			0x0C
#define BSPI_RAF_WATERMARK_REG			0x10
#define BSPI_RAF_STATUS_REG			0x14
#define BSPI_RAF_READ_DATA_REG			0x18
#define BSPI_RAF_WORD_CNT_REG			0x1C
#define BSPI_RAF_CURR_ADDR_REG			0x20

/*
 * Register access macros
 */
#define REG_RD(x)	readl(x)
#define REG_WR(x, y)	writel((y), (x))
#define REG_CLR(x, y)	REG_WR((x), REG_RD(x) & ~(y))
#define REG_SET(x, y)	REG_WR((x), REG_RD(x) | (y))

/* State */
enum bcm_qspi_state {
	QSPI_STATE_DISABLED,
	QSPI_STATE_MSPI,
	QSPI_STATE_BSPI
};

/* QSPI private data */
struct bcmspi_priv {
	/* Slave entry */
	struct spi_slave slave;

	/* Specified SPI parameters */
	unsigned int max_hz;
	unsigned int spi_mode;

	/* State */
	enum bcm_qspi_state state;
	u8 bspi_op;
	u32 bspi_addr;
	int mspi_16bit;
	int mode_4byte;

	/* Registers */
	void *mspi_hw;
	void *bspi_hw;
	void *bspi_hw_raf;
	void *cru_hw;
};

/* Macro to get the private data */
#define to_qspi_slave(s) container_of(s, struct bcmspi_priv, slave)

int iproc_qspi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	if (bus == CONFIG_IPROC_QSPI_BUS && cs == CONFIG_IPROC_QSPI_CS)
		return 1;
	return 0;
}

struct spi_slave *iproc_qspi_setup_slave(unsigned int bus,
					 unsigned int cs,
					 unsigned int max_hz,
					 unsigned int mode)
{
	struct bcmspi_priv *priv;
	unsigned int spbr;

	if (!spi_cs_is_valid(bus, cs)) {
		error("Wrong QSPI bus (%d) cs (%d)!\n", bus, cs);
		return NULL;
	}

	priv = (struct bcmspi_priv *)spi_alloc_slave(struct bcmspi_priv,
						     bus, cs);
	if (!priv) {
		error("No memory for QSPI slave!\n");
		return NULL;
	}

	priv->max_hz = max_hz;
	priv->spi_mode = mode;
	priv->state = QSPI_STATE_DISABLED;
	priv->mode_4byte = 0;
	priv->bspi_hw = (void *)(QSPI_REG_BASE + 0x000);
	priv->bspi_hw_raf = (void *)(QSPI_REG_BASE + 0x100);
	priv->mspi_hw = (void *)(QSPI_REG_BASE + 0x200);
	priv->cru_hw = (void *)CRU_CONTROL_REG;

	/* BSPI: clock configuration */
	REG_CLR(priv->cru_hw, 0x00000006);
	REG_RD(priv->cru_hw);	/* Need to read back */
	if (priv->max_hz >= 62500000)
		REG_SET(priv->cru_hw, 0x00000006);
	else if (priv->max_hz >= 50000000)
		REG_SET(priv->cru_hw, 0x00000002);
	else if (priv->max_hz >= 31250000)
		REG_SET(priv->cru_hw, 0x00000004);
	REG_RD(priv->cru_hw);	/* Need to read back */

	/* BSPI: configure for dual/quad mode */
	if (CONFIG_IPROC_BSPI_DATA_LANES != 1 ||
	    CONFIG_IPROC_BSPI_ADDR_LANES != 1) {
		/* Disable flex mode first */
		REG_WR(priv->bspi_hw + BSPI_FLEX_MODE_ENABLE_REG, 0);

		/* Data / Address lanes */
		REG_WR(priv->bspi_hw + BSPI_BITS_PER_CYCLE_REG, 0);
		if (CONFIG_IPROC_BSPI_DATA_LANES == 4) {
			REG_SET(priv->bspi_hw + BSPI_BITS_PER_CYCLE_REG,
				2 << BSPI_BITS_PER_CYCLE_DATA_SHIFT);
		} else {
			REG_SET(priv->bspi_hw + BSPI_BITS_PER_CYCLE_REG,
				(CONFIG_IPROC_BSPI_DATA_LANES - 1)
					<< BSPI_BITS_PER_CYCLE_DATA_SHIFT);
		}
		if (CONFIG_IPROC_BSPI_ADDR_LANES == 4) {
			REG_SET(priv->bspi_hw + BSPI_BITS_PER_CYCLE_REG,
				2 << BSPI_BITS_PER_CYCLE_ADDR_SHIFT);
		} else {
			REG_SET(priv->bspi_hw + BSPI_BITS_PER_CYCLE_REG,
				(CONFIG_IPROC_BSPI_ADDR_LANES - 1)
					<< BSPI_BITS_PER_CYCLE_ADDR_SHIFT);
		}

		/* Dummy cycles */
		REG_CLR(priv->bspi_hw + BSPI_BITS_PER_PHASE_REG, 0xFF);
		REG_SET(priv->bspi_hw + BSPI_BITS_PER_PHASE_REG,
			CONFIG_IPROC_BSPI_READ_DUMMY_CYCLES);

		/* Command byte for BSPI */
		REG_WR(priv->bspi_hw + BSPI_CMD_AND_MODE_BYTE_REG,
		       CONFIG_IPROC_BSPI_READ_CMD);

		/* Enable flex mode to take effect */
		REG_WR(priv->bspi_hw + BSPI_FLEX_MODE_ENABLE_REG, 1);
	}

	/* MSPI: Basic hardware initialization */
	REG_WR(priv->mspi_hw + MSPI_SPCR1_LSB_REG, 0);
	REG_WR(priv->mspi_hw + MSPI_SPCR1_MSB_REG, 0);
	REG_WR(priv->mspi_hw + MSPI_NEWQP_REG, 0);
	REG_WR(priv->mspi_hw + MSPI_ENDQP_REG, 0);
	REG_WR(priv->mspi_hw + MSPI_SPCR2_REG, 0);

	/* MSPI: SCK configuration */
	spbr = (QSPI_AXI_CLK - 1) / (2 * priv->max_hz) + 1;
	REG_WR(priv->mspi_hw + MSPI_SPCR0_LSB_REG,
	       max(min(spbr, SPBR_MAX), SPBR_MIN));

	/* MSPI: Mode configuration (8 bits by default) */
	priv->mspi_16bit = 0;
	REG_WR(priv->mspi_hw + MSPI_SPCR0_MSB_REG,
	       0x80 |			/* Master */
	       (8 << 2) |		/* 8 bits per word */
	       (priv->spi_mode & 3));	/* mode: CPOL / CPHA */

	return &priv->slave;
}

void iproc_qspi_free_slave(struct spi_slave *slave)
{
	struct bcmspi_priv *priv;
	if (slave == NULL)
		return;
	priv = to_qspi_slave(slave);
	free(priv);
}

static void bspi_flush_prefetch_buffers(struct bcmspi_priv *priv)
{
	REG_WR(priv->bspi_hw + BSPI_B0_CTRL_REG, 0);
	REG_WR(priv->bspi_hw + BSPI_B1_CTRL_REG, 0);
	REG_WR(priv->bspi_hw + BSPI_B0_CTRL_REG, 1);
	REG_WR(priv->bspi_hw + BSPI_B1_CTRL_REG, 1);
}

static int bcmspi_enable_bspi(struct bcmspi_priv *priv)
{
	if (priv->state == QSPI_STATE_BSPI)
		return 0;

	/* Disable write lock */
	REG_WR(priv->mspi_hw + MSPI_WRITE_LOCK_REG, 0);

	/* Flush prefetch buffers */
	bspi_flush_prefetch_buffers(priv);

	/* Switch to BSPI */
	REG_WR(priv->bspi_hw + BSPI_MAST_N_BOOT_CTRL_REG, 0);

	/* Update state */
	priv->state = QSPI_STATE_BSPI;

	return 0;
}

static int bcmspi_disable_bspi(struct bcmspi_priv *priv)
{
	if (priv->state == QSPI_STATE_MSPI)
		return 0;

	/* Switch to MSPI if not yet */
	if ((REG_RD(priv->bspi_hw + BSPI_MAST_N_BOOT_CTRL_REG) & 1) == 0) {
		unsigned long start = get_timer(0);
		while (get_timer(start) <
		       QSPI_WAIT_TIMEOUT_MS * CONFIG_SYS_HZ / 1000) {
			if ((REG_RD(priv->bspi_hw + BSPI_BUSY_STATUS_REG) & 1)
			    == 0) {
				REG_WR(priv->bspi_hw +
				       BSPI_MAST_N_BOOT_CTRL_REG, 1);
				udelay(1);
				break;
			}
			udelay(1);
		}
		if ((REG_RD(priv->bspi_hw + BSPI_MAST_N_BOOT_CTRL_REG) & 1)
		    != 1)
			return -1;
	}

	/* Update state */
	priv->state = QSPI_STATE_MSPI;

	return 0;
}

int iproc_qspi_claim_bus(struct spi_slave *slave)
{
	struct bcmspi_priv *priv;

	if (slave == NULL)
		return -1;

	priv = to_qspi_slave(slave);
	if (priv->state != QSPI_STATE_DISABLED)
		return 0;	/* Already enabled */

	/* Switch to BSPI by default */
	if (bcmspi_enable_bspi(priv) != 0)
		return -1;

	return 0;
}

void iproc_qspi_release_bus(struct spi_slave *slave)
{
	struct bcmspi_priv *priv;

	if (slave == NULL)
		return;

	priv = to_qspi_slave(slave);
	if (priv->state == QSPI_STATE_DISABLED)
		return;

	/* Make sure no operation is in progress */
	REG_WR(priv->mspi_hw + MSPI_SPCR2_REG, 0);
	udelay(1);

	/* Switch to BSPI */
	bcmspi_enable_bspi(priv);

	/* Update state */
	priv->state = QSPI_STATE_DISABLED;
}

static void bspi_set_4byte_mode(struct bcmspi_priv *priv, int enable)
{
	/* Disable flex mode first */
	REG_WR(priv->bspi_hw + BSPI_FLEX_MODE_ENABLE_REG, 0);

	if (enable) {
		/* Enable 32-bit address */
		REG_SET(priv->bspi_hw + BSPI_BITS_PER_PHASE_REG,
			BSPI_BITS_PER_PHASE_ADDR_MARK);

		/* Enable flex mode to take effect */
		REG_WR(priv->bspi_hw + BSPI_FLEX_MODE_ENABLE_REG, 1);
	} else {
		/* Disable 32-bit address */
		REG_CLR(priv->bspi_hw + BSPI_BITS_PER_PHASE_REG,
			BSPI_BITS_PER_PHASE_ADDR_MARK);

		/* Clear upper address byte */
		REG_WR(priv->bspi_hw + BSPI_FLASH_UPPER_ADDR_BYTE_REG, 0);

		/*
		 * Flush prefetch buffers since 32MB window BSPI
		 * could be used
		 */
		bspi_flush_prefetch_buffers(priv);
	}

	/* Record current mode */
	priv->mode_4byte = enable;
}

#define DWORD_ALIGNED(a) (!(((unsigned long)(a)) & 3))

static void bspi_read_via_raf(struct bcmspi_priv *priv, u8 *rx, uint bytes)
{
	u32 status;
	uint words;
	int aligned;

	/* Flush data from the previous session (unlikely) */
	for (;;) {
		status = REG_RD(priv->bspi_hw_raf + BSPI_RAF_STATUS_REG);
		if (!(status & BSPI_RAF_STATUS_FIFO_EMPTY_MASK)) {
			REG_RD(priv->bspi_hw_raf + BSPI_RAF_READ_DATA_REG);
			continue;
		}
		if (!(status & BSPI_RAF_STATUS_SESSION_BUSY_MASK))
			break;
	}

	/* Transfer is in words */
	words = (bytes + 3) / 4;

	/* Setup hardware */
	if (priv->mode_4byte) {
		u32 val = priv->bspi_addr & 0xFF000000;
		if (val != REG_RD(priv->bspi_hw +
				  BSPI_FLASH_UPPER_ADDR_BYTE_REG)) {
			REG_WR(priv->bspi_hw + BSPI_FLASH_UPPER_ADDR_BYTE_REG,
			       val);
			bspi_flush_prefetch_buffers(priv);
		}
	}
	REG_WR(priv->bspi_hw_raf + BSPI_RAF_START_ADDRESS_REG,
	       priv->bspi_addr & 0x00FFFFFF);
	REG_WR(priv->bspi_hw_raf + BSPI_RAF_NUM_WORDS_REG, words);
	REG_WR(priv->bspi_hw_raf + BSPI_RAF_WATERMARK_REG, 0);

	/* Kick off */
	REG_WR(priv->bspi_hw_raf + BSPI_RAF_CTRL_REG,
	       BSPI_RAF_CONTROL_START_MASK);

	/* Reading the data */
	aligned = DWORD_ALIGNED(rx);
	while (bytes) {
		status = REG_RD(priv->bspi_hw_raf + BSPI_RAF_STATUS_REG);
		if (!(status & BSPI_RAF_STATUS_FIFO_EMPTY_MASK)) {
			u32 data = le32_to_cpu(REG_RD(priv->bspi_hw_raf +
						      BSPI_RAF_READ_DATA_REG));

			/* Check if we can use the whole word */
			if (aligned && bytes >= 4) {
				/*
				 * RAF is LE only,
				 * convert data to host endianness
				 */
				*(u32 *)rx = le32_to_cpu(data);
				rx += 4;
				bytes -= 4;
			} else {
				uint chunk = min(bytes, 4U);

				/* Read out bytes one by one */
				while (chunk) {
					*rx++ = (u8) data;
					data >>= 8;
					chunk--;
					bytes--;
				}
			}

			continue;
		}
		if (!(status & BSPI_RAF_STATUS_SESSION_BUSY_MASK)) {
			/* FIFO is empty and the session is done */
			break;
		}
	}
}

static int bspi_emulate_flash_read(struct bcmspi_priv *priv,
				   const u8 *tx, u8 *rx, uint bytes,
				   unsigned long flags)
{
	int idx = priv->mode_4byte ? 2 : 1;

	/* Check the address */
	if (flags & SPI_XFER_BEGIN) {
		u32 addr;

		/* Drop if the first transfer doesn't contain full address */
		if (bytes < idx + 3 + 1)
			return -1;

		/* Flash offset - lower 24 bits */
		addr = (tx[idx] << 16) | (tx[idx + 1] << 8) | tx[idx + 2];

		/* Flash offset - upper 8 bits */
		if (priv->mode_4byte)
			addr |= tx[1] << 24;

		/* Remaining length for data (excluding one dummy cycle) */
		bytes -= idx + 3 + 1;
		if (rx)
			rx += idx + 3 + 1;

		/* non-aligned transfers are handled by MSPI */
		if (!DWORD_ALIGNED(addr))
			return -1;

		/* Switch to BSPI */
		if (bcmspi_enable_bspi(priv) != 0)
			return -1;

		/* Record BSPI status */
		priv->bspi_op = OPCODE_FAST_READ;
		priv->bspi_addr = addr;
	}

	/* Transfer data if any */
	while (bytes && rx) {
		/* Special handing since RAF cannot go across 16MB boundary */
		uint trans = bytes;

		/*
		 * Divide into multiple transfers if it goes
		 * across the boundary
		 */
		if (priv->mode_4byte &&
		    (priv->bspi_addr >> 24) !=
		    ((priv->bspi_addr + bytes) >> 24)) {
			/*
			 * Limit this transfer to not go beyond
			 * 16MB boundary
			 */
			trans = 0x1000000 - (priv->bspi_addr & 0xFFFFFF);
		}
		bspi_read_via_raf(priv, rx, trans);
		priv->bspi_addr += trans;
		rx += trans;
		bytes -= trans;
	}

	/* Flush prefetch buffers at the end */
	if (flags & SPI_XFER_END)
		bspi_flush_prefetch_buffers(priv);

	return 0;
}

static int bspi_emulate_flash_rdsr(struct bcmspi_priv *priv,
				   const u8 *tx, u8 *rx, uint bytes,
				   unsigned long flags)
{
	/* Only emulate the status register if it was a BSPI read */
	if (priv->state != QSPI_STATE_BSPI)
		return -1;

	/* Handle for the first transfer */
	if (flags & SPI_XFER_BEGIN) {
		/* Record status */
		priv->bspi_op = OPCODE_RDSR;

		/* Skip the first byte: command */
		bytes--;
		rx++;
	}

	/* Fill the rx data with 0 */
	while (bytes)
		*rx++ = 0x00;

	return 0;
}

static int mspi_xfer(struct bcmspi_priv *priv, uint bytes,
		     const u8 *tx, u8 *rx, int end)
{
	if (bytes & 1) {
		/* Use 8-bit queue for odd-bytes transfer */
		if (priv->mspi_16bit) {
			REG_SET(priv->mspi_hw + MSPI_SPCR0_MSB_REG,
				MSPI_SPCR0_MSB_BITS_8);
			priv->mspi_16bit = 0;
		}
	} else {
		/* Use 16-bit queue for even-bytes transfer */
		if (!priv->mspi_16bit) {
			REG_CLR(priv->mspi_hw + MSPI_SPCR0_MSB_REG,
				MSPI_SPCR0_MSB_BITS_8);
			priv->mspi_16bit = 1;
		}
	}

	while (bytes) {
		uint chunk;
		uint queues;
		unsigned long start;
		uint i;

		/* Separate code for 16bit and 8bit transfers for performance */
		if (priv->mspi_16bit) {
			/* Determine how many bytes to process this time */
			chunk = min(bytes, NUM_CDRAM * 2);
			queues = (chunk - 1) / 2 + 1;
			bytes -= chunk;

			/* Fill CDRAMs */
			for (i = 0; i < queues; i++)
				REG_WR(priv->mspi_hw + MSPI_CDRAM_REG +
				       (i << 2), 0xc2);

			/* Fill TXRAMs */
			for (i = 0; i < chunk; i++)
				REG_WR(priv->mspi_hw + MSPI_TXRAM_REG +
				       (i << 2), tx ? tx[i] : 0xff);
		} else {
			/* Determine how many bytes to process this time */
			chunk = min(bytes, NUM_CDRAM);
			queues = chunk;
			bytes -= chunk;

			/* Fill CDRAMs and TXRAMS */
			for (i = 0; i < chunk; i++) {
				REG_WR(priv->mspi_hw + MSPI_CDRAM_REG +
				       (i << 2), 0x82);
				REG_WR(priv->mspi_hw + MSPI_TXRAM_REG +
				       (i << 3), tx ? tx[i] : 0xff);
			}
		}

		/* Setup queue pointers */
		REG_WR(priv->mspi_hw + MSPI_NEWQP_REG, 0);
		REG_WR(priv->mspi_hw + MSPI_ENDQP_REG, queues - 1);

		/* Deassert CS if requested and it's the last transfer */
		if (bytes == 0 && end)
			REG_CLR(priv->mspi_hw + MSPI_CDRAM_REG +
				((queues - 1) << 2), 0x80);

		/* Kick off */
		REG_WR(priv->mspi_hw + MSPI_STATUS_REG, 0);
		REG_WR(priv->mspi_hw + MSPI_SPCR2_REG, 0xc0);	/* cont | spe */

		/* Wait for completion */
		start = get_timer(0);
		while (get_timer(start) <
		       QSPI_WAIT_TIMEOUT_MS * CONFIG_SYS_HZ / 1000) {
			if (REG_RD(priv->mspi_hw + MSPI_STATUS_REG) & 1)
				break;
		}
		if ((REG_RD(priv->mspi_hw + MSPI_STATUS_REG) & 1) == 0)
			return -1;

		/* Read data out */
		if (rx) {
			if (priv->mspi_16bit) {
				for (i = 0; i < chunk; i++) {
					rx[i] = REG_RD(priv->mspi_hw +
						       MSPI_RXRAM_REG +
						       (i << 2))
						& 0xff;
				}
			} else {
				for (i = 0; i < chunk; i++) {
					rx[i] = REG_RD(priv->mspi_hw +
						       MSPI_RXRAM_REG +
						       (((i << 1) + 1) << 2))
						& 0xff;
				}
			}
		}

		/* Advance pointers */
		if (tx)
			tx += chunk;
		if (rx)
			rx += chunk;
	}

	return 0;
}

int iproc_qspi_xfer(struct spi_slave *slave, unsigned int bitlen,
		    const void *dout, void *din, unsigned long flags)
{
	struct bcmspi_priv *priv;
	const u8 *tx = dout;
	u8 *rx = din;
	uint bytes = bitlen / 8;
	int ret = 0;

	if (slave == NULL)
		return -1;

	priv = to_qspi_slave(slave);
	if (priv->state == QSPI_STATE_DISABLED)
		return -1;

	/* we can only do 8 bit transfers */
	if (bitlen % 8)
		return -1;

	/* Check if we can make use of BSPI */
	if (flags & SPI_XFER_BEGIN) {
		/* We only use BSPI if the first transfer contains command */
		if (bytes) {
			switch (tx[0]) {
			case OPCODE_FAST_READ:
				if (bspi_emulate_flash_read
				    (priv, tx, rx, bytes, flags) == 0) {
					return 0;
				}
				break;
			case OPCODE_RDSR:
				if (bspi_emulate_flash_rdsr
				    (priv, tx, rx, bytes, flags) == 0) {
					return 0;
				}
				break;
			case OPCODE_EN4B:
				bspi_set_4byte_mode(priv, 1);
				break;
			case OPCODE_EX4B:
				bspi_set_4byte_mode(priv, 0);
				break;
			case OPCODE_BRWR:
				bspi_set_4byte_mode(priv, tx[1] ? 1 : 0);
				break;
			default:
				break;
			}
		}

	} else if (priv->state == QSPI_STATE_BSPI) {
		/* It's a following BSPI operation */
		switch (priv->bspi_op) {
		case OPCODE_FAST_READ:
			if (bspi_emulate_flash_read(priv, tx, rx, bytes, flags)
			    == 0)
				return 0;
			break;
		case OPCODE_RDSR:
			if (bspi_emulate_flash_rdsr(priv, tx, rx, bytes, flags)
			    == 0) {
				return 0;
			}
			break;
		default:
			break;
		}
		return -1;
	}

	/* MSPI: Enable write lock at the beginning */
	if (flags & SPI_XFER_BEGIN) {
		/* Switch to MSPI if not yet */
		if (bcmspi_disable_bspi(priv) != 0)
			return -1;

		REG_WR(priv->mspi_hw + MSPI_WRITE_LOCK_REG, 1);
	}

	/* MSPI: Transfer it */
	if (bytes)
		ret = mspi_xfer(priv, bytes, tx, rx, (flags & SPI_XFER_END));

	/* MSPI: Disable write lock if it's done */
	if (flags & SPI_XFER_END)
		REG_WR(priv->mspi_hw + MSPI_WRITE_LOCK_REG, 0);

	return ret;
}
