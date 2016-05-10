/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */
#include <spi.h>

#ifndef _IPROC_PL022_SPI_H_
#define _IPROC_PL022_SPI_H_

int iproc_pl022_spi_cs_is_valid(unsigned int bus, unsigned int cs);

struct spi_slave *iproc_pl022_spi_setup_slave(unsigned int bus,
					      unsigned int cs,
					      unsigned int max_hz,
					      unsigned int mode);

void iproc_pl022_spi_free_slave(struct spi_slave *slave);

int iproc_pl022_spi_claim_bus(struct spi_slave *slave);

void iproc_pl022_spi_release_bus(struct spi_slave *slave);

int iproc_pl022_spi_xfer(struct spi_slave *slave, unsigned int bitlen,
			 const void *dout, void *din, unsigned long flags);

#endif	/* _IPROC_PL022_SPI_H_ */
