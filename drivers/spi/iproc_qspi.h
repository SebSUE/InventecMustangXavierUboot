/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */
#include <spi.h>

#ifndef _IPROC_QSPI_H_
#define _IPROC_QSPI_H_

int iproc_qspi_cs_is_valid(unsigned int bus, unsigned int cs);

struct spi_slave *iproc_qspi_setup_slave(unsigned int bus,
					 unsigned int cs,
					 unsigned int max_hz,
					 unsigned int mode);

void iproc_qspi_free_slave(struct spi_slave *slave);

int iproc_qspi_claim_bus(struct spi_slave *slave);

void iproc_qspi_release_bus(struct spi_slave *slave);

int iproc_qspi_xfer(struct spi_slave *slave, unsigned int bitlen,
		    const void *dout, void *din, unsigned long flags);

#endif	/* _IPROC_QSPI_H_ */
