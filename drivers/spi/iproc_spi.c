/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */
#include <common.h>
#include <spi.h>
#include "iproc_pl022_spi.h"
#include "iproc_qspi.h"

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
#ifdef CONFIG_IPROC_QSPI
	if (bus == CONFIG_IPROC_QSPI_BUS)
		return iproc_qspi_cs_is_valid(bus, cs);
#endif
#ifdef CONFIG_IPROC_PL022_SPI
	if (bus == CONFIG_IPROC_PL022_SPI_BUS)
		return iproc_pl022_spi_cs_is_valid(bus, cs);
#endif
	return 0;
}

void spi_init(void)
{
	/* Do nothing */
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
#ifdef CONFIG_IPROC_QSPI
	if (bus == CONFIG_IPROC_QSPI_BUS)
		return iproc_qspi_setup_slave(bus, cs, max_hz, mode);
#endif
#ifdef CONFIG_IPROC_PL022_SPI
	if (bus == CONFIG_IPROC_PL022_SPI_BUS)
		return iproc_pl022_spi_setup_slave(bus, cs, max_hz, mode);
#endif
	return NULL;
}

void spi_free_slave(struct spi_slave *slave)
{
#ifdef CONFIG_IPROC_QSPI
	if (slave->bus == CONFIG_IPROC_QSPI_BUS)
		iproc_qspi_free_slave(slave);
#endif
#ifdef CONFIG_IPROC_PL022_SPI
	if (slave->bus == CONFIG_IPROC_PL022_SPI_BUS)
		iproc_pl022_spi_free_slave(slave);
#endif
}

int spi_claim_bus(struct spi_slave *slave)
{
#ifdef CONFIG_IPROC_QSPI
	if (slave->bus == CONFIG_IPROC_QSPI_BUS)
		return iproc_qspi_claim_bus(slave);
#endif
#ifdef CONFIG_IPROC_PL022_SPI
	if (slave->bus == CONFIG_IPROC_PL022_SPI_BUS)
		return iproc_pl022_spi_claim_bus(slave);
#endif
	return -1;
}

void spi_release_bus(struct spi_slave *slave)
{
#ifdef CONFIG_IPROC_QSPI
	if (slave->bus == CONFIG_IPROC_QSPI_BUS)
		iproc_qspi_release_bus(slave);
#endif
#ifdef CONFIG_IPROC_PL022_SPI
	if (slave->bus == CONFIG_IPROC_PL022_SPI_BUS)
		iproc_pl022_spi_release_bus(slave);
#endif
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
#ifdef CONFIG_IPROC_QSPI
	if (slave->bus == CONFIG_IPROC_QSPI_BUS)
		return iproc_qspi_xfer(slave, bitlen, dout, din, flags);
#endif
#ifdef CONFIG_IPROC_PL022_SPI
	if (slave->bus == CONFIG_IPROC_PL022_SPI_BUS)
		return iproc_pl022_spi_xfer(slave, bitlen, dout, din, flags);
#endif
	return -1;
}
