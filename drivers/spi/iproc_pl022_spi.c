/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include "iproc_pl022_spi.h"

#ifndef CONFIG_IPROC_PL022_SPI_BUS
#error CONFIG_IPROC_PL022_SPI_BUS not defined
#endif

#define IPROC_SPI0_SSP_PID0 0x18028fe0
#define IPROC_SPI0_SSP_PID1 0x18028fe4
#define IPROC_SPI0_SSP_PID2 0x18028fe8
#define IPROC_SPI0_SSP_PID3 0x18028fec

#define IPROC_SPI0_SSPCR0 0x18028000
#define IPROC_SPI1_SSPCR0 0x18029000
#define IPROC_SSPCR0_SCR_SHIFT		8
#define IPROC_SSPCR0_SCR_MASK		0x0000FF00
#define IPROC_SSPCR0_SPH_MASK		0x00000080
#define IPROC_SSPCR0_SPO_MASK		0x00000040
#define IPROC_SSPCR0_FRF_SHIFT		4
#define IPROC_SSPCR0_FRF_MASK		0x00000030
#define IPROC_SSPCR0_DSS_SHIFT		0
#define IPROC_SSPCR0_DSS_MASK		0x0000000F

#define IPROC_SPI0_SSPCR1 0x18028004
#define IPROC_SSPCR1_SSE_SHIFT 1

#define IPROC_SPI0_SSPCPSR 0x18028010

#define IPROC_SPI0_SSPSR 0x1802800c
#define IPROC_SSPSR_BSY_MASK  0x10
#define IPROC_SSPSR_RNE_MASK  0x4
#define IPROC_SSPSR_TNF_MASK  0x2

#define IPROC_SPI0_SSPDR 0x18028008

/* base address must be address of SPI0, spi: 0-5 */
#define SPI_ADDR(base_addr, spi) \
	(base_addr + (spi) * (IPROC_SPI1_SSPCR0 - IPROC_SPI0_SSPCR0))

#define SCR_MAX         0xFF
#define CPSR_MAX        0xFE

/* pending rx bytes */
static uint32_t pending_len_rx;

int iproc_pl022_spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	if (bus == CONFIG_IPROC_PL022_SPI_BUS)
		return 1;
	return 0;
}

struct spi_slave *iproc_pl022_spi_setup_slave(
		unsigned int bus, unsigned int cs, unsigned int max_hz,
		unsigned int mode) {
	struct spi_slave *slave;
	uint16_t scr, prescaler, cr0, cpsr = 0;

	if (!iproc_pl022_spi_cs_is_valid(bus, cs)) {
		error("Wrong PL022 bus (%d)!\n", bus);
		return NULL;
	}

	slave = spi_alloc_slave_base(bus, cs);
	if (!slave) {
		error("No memory for PL022 slave!\n");
		return NULL;
	}

	/* PID should be 0x00041022 */
	if ((readl(SPI_ADDR(IPROC_SPI0_SSP_PID0, cs)) == 0x22) &&
	    (readl(SPI_ADDR(IPROC_SPI0_SSP_PID1, cs)) == 0x10) &&
	    ((readl(SPI_ADDR(IPROC_SPI0_SSP_PID2, cs)) & 0xf) == 0x04) &&
	    (readl(SPI_ADDR(IPROC_SPI0_SSP_PID3, cs)) == 0x00)) {
		printf("PL022 SPI controller found!\n");
	} else {
		free(slave);
		return NULL;
	}

	/* Disable SSP */
	writel(0x0, SPI_ADDR(IPROC_SPI0_SSPCR1, slave->cs));

	/* Set requested polarity and 8 bit word length */
	slave->wordlen = 8;
	cr0 = 7; /* 8 bits */
	if (mode & SPI_CPHA)
		cr0 |= IPROC_SSPCR0_SPH_MASK;
	if (mode & SPI_CPOL)
		cr0 |= IPROC_SSPCR0_SPO_MASK;

	/* Program the SSPClk frequency */
	prescaler = CONFIG_IPROC_SPI_CLK / max_hz;

	if (prescaler <= CPSR_MAX) {
		cpsr = prescaler;
		scr = 1;
	} else {
		for (scr = 2; scr <= SCR_MAX; scr++) {
			if (!(prescaler % scr)) {
				cpsr = prescaler / scr;
				if (cpsr <= CPSR_MAX)
					break;
			}
		}

		if (scr > SCR_MAX) {
			scr = SCR_MAX;
			cpsr = prescaler / scr;
			cpsr &= CPSR_MAX;
		}
	}

	if (cpsr & 0x1)
		cpsr++;

	writel(cpsr, SPI_ADDR(IPROC_SPI0_SSPCPSR, slave->cs));

	writel(cr0 | (IPROC_SSPCR0_SCR_MASK &
			((scr - 1) << IPROC_SSPCR0_SCR_SHIFT)),
	       SPI_ADDR(IPROC_SPI0_SSPCR0, slave->cs));

	return slave;
}

void iproc_pl022_spi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

static int iproc_pl022_spi_set_wordlen(struct spi_slave *slave)
{
	uint32_t cr0;

	if ((slave->wordlen != 16) && (slave->wordlen != 8)) {
		error("SPI word length (%d) not supported!\n", slave->wordlen);
		return -1;
	}
	cr0 = readl(SPI_ADDR(IPROC_SPI0_SSPCR0, slave->cs));
	cr0 &= ~IPROC_SSPCR0_DSS_MASK;
	cr0 |= slave->wordlen - 1;
	writel(cr0, SPI_ADDR(IPROC_SPI0_SSPCR0, slave->cs));
	return 0;
}

static void iproc_pl022_spi_flush(struct spi_slave *slave)
{
	uint32_t stats;

	do {
		stats = readl(SPI_ADDR(IPROC_SPI0_SSPSR, slave->cs));
		/* receive fifo not empty */
		if (stats & IPROC_SSPSR_RNE_MASK) {
			/* Clear Rx FIFO */
			readl(SPI_ADDR(IPROC_SPI0_SSPDR,
				       slave->cs));
		}
	} while ((stats & IPROC_SSPSR_BSY_MASK) ||
		 (stats & IPROC_SSPSR_RNE_MASK));

	pending_len_rx = 0;
}

int iproc_pl022_spi_claim_bus(struct spi_slave *slave)
{
	if (iproc_pl022_spi_set_wordlen(slave))
		return -1;

	/* Enable the SPI hardware */
	if (slave->bus == CONFIG_IPROC_PL022_SPI_BUS)
		writel(1 << IPROC_SSPCR1_SSE_SHIFT,
		       SPI_ADDR(IPROC_SPI0_SSPCR1, slave->cs));

	pending_len_rx  = 0;

	return 0;
}

void iproc_pl022_spi_release_bus(struct spi_slave *slave)
{
	/* Disable SPI */
	if (slave->bus == CONFIG_IPROC_PL022_SPI_BUS)
		writel(0x0, SPI_ADDR(IPROC_SPI0_SSPCR1, slave->cs));
}

/*
 * Support multiple invokes for one frame, but device speed has to be low and
 * no delay between invokes.
 *
 * Flash API send commands using multiple invokes of spi_xfer.
 * PL022 deasserts CS when it reaches the end of TX FIFO, so we attempt to
 * have as much data in the TX FIFO when this function returns, and leave the
 * read of dummy RX bytes to be done on the next invoke, so the CS will not be
 * deasserted during a transaction spanning over multiple invokes of spi_xfer.
 */
int iproc_pl022_spi_xfer(struct spi_slave *slave, unsigned int bitlen,
			 const void *dout, void *din, unsigned long flags)
{
	uint32_t len_tx = 0, len_rx = 0, len;
	const uint8_t *txp = dout;
	uint8_t *rxp = din;
	uint32_t value;

	if (bitlen == 0) {
		if (flags & SPI_XFER_END)
			iproc_pl022_spi_flush(slave);
		return 0;
	}

	if (bitlen % slave->wordlen) {
		error("Wrong data size (%d) in SPI xfer request!\n", bitlen);
		return -1;
	}

	len = bitlen / slave->wordlen;

	/* Clear buffer */
	if (flags & SPI_XFER_BEGIN)
		iproc_pl022_spi_flush(slave);

	while ((len_tx < len) || (len_rx < len) || pending_len_rx) {
		/* Tx */
		while ((len_tx < len) &&
		       (readl(SPI_ADDR(IPROC_SPI0_SSPSR, slave->cs)) &
			IPROC_SSPSR_TNF_MASK)) {
			/* transmit fifo not full */
			if (slave->wordlen == 16) {
				if (txp) {
					value = *((uint16_t *)txp);
					txp += 2;
				} else {
					value = 0;
				}
			} else {
				if (txp) {
					value = *txp;
					txp++;
				} else {
					value = 0;
				}
			}
			writel(value, SPI_ADDR(IPROC_SPI0_SSPDR, slave->cs));
			len_tx++;
		}
		/*
		 * If not at the end of the SPI transfer, do not read data
		 * from RX fifo, because PL022 deasserts the CS when the
		 * TX FIFO gets empty.
		 */
		if (!rxp && !(flags & SPI_XFER_END) && (len_tx == len)) {
			pending_len_rx += len - len_rx;
			break;
		}
		/* Rx */
		if (((len_rx < len) || pending_len_rx) &&
		    (readl(SPI_ADDR(IPROC_SPI0_SSPSR, slave->cs)) &
		    IPROC_SSPSR_RNE_MASK)) {
			/* receive fifo not empty */
			value = readl(SPI_ADDR(IPROC_SPI0_SSPDR, slave->cs));
			if (pending_len_rx) {
				pending_len_rx--;
			} else {
				len_rx++;
				if (rxp) {
					if (slave->wordlen == 16) {
						*((uint16_t *)rxp) =
							(uint16_t)value;
						rxp += 2;
					} else {
						*rxp = (uint8_t)value;
						rxp++;
					}
				}
			}
		}
	}

	/* Clear buffer */
	if (flags & SPI_XFER_END)
		iproc_pl022_spi_flush(slave);

	return 0;
}
