/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#ifndef __GIC_H
#define __GIC_H

#include <linux/types.h>
#include <asm/iproc-common/sysmap.h>

/* GIC (Generic Interrupt controller) definitions */

struct gic_data_struct {
	uint32_t cpuid;
	uint32_t intid;
	void *data;
};

/* GIC (Generic Interrupt controller) CPU interface registers */
#define GIC_CI_CTRL_EN				0x00000001
#define GIC_CI_PMR_PRIO_MASK		0x000000FF
#define GIC_CI_BPR_BP_MASK			0x00000003
#define GIC_CI_IAR_INTID_MASK		0x000003FF
#define GIC_CI_IAR_CPUID_MASK		0x00001C00
#define GIC_CI_IAR_CPUID_OFFSET				10
#define GIC_CI_EOIR_INTID_MASK		0x000003FF
#define GIC_CI_EOIR_CPUID_MASK		0x00001C00
#define GIC_CI_EOIR_CPUID_OFFSET			10
#define GIC_CI_RPR_PRIO_MASK		0x000000FF
#define GIC_CI_HPIR_PENDID_MASK		0x000003FF
#define GIC_CI_HPIR_CPUID_MASK		0x00001C00
#define GIC_CI_HPIR_CPUID_OFFSET			10
#define GIC_CI_ABPR_BP_MASK			0x00000003



struct gic_ci_regs {
	uint32_t control;	/* Control reg */
	uint32_t pmr;		/* Interrupt Priority mask reg */
	uint32_t bpr;		/* Binary point reg */
	uint32_t iar;		/* interrupt acknowledge reg */
	uint32_t eoir;		/* End of interrupt reg */
	uint32_t rpr;		/* running priority register */
	uint32_t hpir;		/* Highest pending interrupt register */
	uint32_t abpr;		/* Aliased Non-Secure Binary point register */
	uint32_t rsvd[55];
	uint32_t idr;		/* CPU Interface Implementer Id reg */
};

#define GIC_DIST_CTRL_S_EN_S				0x00000001
#define GIC_DIST_CTRL_S_EN_NS				0x00000002
#define GIC_DIST_CTRL_NS_EN_NS				0x00000001

#define GIC_DIST_ISR_BIT_SIZE			1
#define GIC_DIST_ISER_BIT_SIZE			1
#define GIC_DIST_ICER_BIT_SIZE			1
#define GIC_DIST_ISPR_BIT_SIZE			1
#define GIC_DIST_ISPR_SECURE			1
#define GIC_DIST_ISPR_NON_SECURE		0
#define GIC_DIST_ICPR_BIT_SIZE			1
#define GIC_DIST_IPR_BIT_SIZE			8
#define GIC_DIST_IPTR_BIT_SIZE			8
#define GIC_DIST_IPTR_CPU0					0x01
#define GIC_DIST_IPTR_CPU1					0x02
#define GIC_DIST_SGIR_ID_MASK				0xF
#define GIC_DIST_SGIR_TR_LIST_MASK			0x00FF0000
#define GIC_DIST_SGIR_TR_LIST_BOFFSET		16
#define GIC_DIST_SGIR_TR_FILT_MASK			0x03000000
#define GIC_DIST_SGIR_TR_FILT_BOFFSET		24
#define GIC_DIST_SGIR_TR_FILT_FW_LIST		0
#define GIC_DIST_SGIR_TR_FILT_FW_ALL_EX_ME	0x01
#define GIC_DIST_SGIR_TR_FILT_FW_ME_ONLY	0x02

#define IPROC_INTR_LEVEL_SENSITIVE					1
#define IPROC_INTR_EDGE_TRIGGERED					2

struct gic_dist_regs {
	uint32_t control;	/* Control reg */
	uint32_t ictr;		/* Interrupt controller type reg */
	uint32_t idr;		/* Distributor Implementer Identification reg*/
	uint32_t rsvd1[1];
	uint32_t rsvd2[28];
	uint32_t isr[8];	/* interrupt security reg */
	uint32_t rsvd3[24];
	uint32_t iser[8];	/* interrupt set-enable reg */
	uint32_t rsvd4[24];
	uint32_t icer[8];	/* interrupt clear-enable reg */
	uint32_t rsvd5[24];
	uint32_t ispr[8];	/* interrupt set-pending reg */
	uint32_t rsvd6[24];
	uint32_t icpr[8];	/* interrupt clear-pending reg */
	uint32_t rsvd7[24];
	uint32_t abr[8];	/* Active bit reg */
	uint32_t rsvd8[24];
	uint32_t rsvd9[32];
	uint32_t ipr[64];	/* Interrupt priority reg */
	uint32_t rsvd10[192];
	uint32_t iptr[64];	/* Interrupt processor targets reg */
	uint32_t rsvd11[192];
	uint32_t icfr[16];	/* Interrupt configuration reg */
	uint32_t rsvd12[48];
	uint32_t ppi_status;    /* PPI status register */
	uint32_t spi_status[7]; /* SPI status register */
	uint32_t rsvd13[24];
	uint32_t rsvd14[32];
	uint32_t rsvd15[64];
	uint32_t sgir;		/* Software generated interrupt reg */
	uint32_t rsvd16[51];
	uint32_t periph_id[8];
	uint32_t component_id[4];
};

/* Function to enable interrupt at distributor level */
void gic_enable_interrupt(uint8_t intid);

/* Function to disable interrupt at distributor level */
void gic_disable_interrupt(uint8_t intid);

/* Function to set Software Generated Interrupt */
void gic_set_sgi(uint8_t intid,
		 uint8_t filter,
		 uint8_t targets);

/* Function to configure the interrupt properties in GIC distributor */
void gic_config_interrupt(uint8_t intid,
			  uint8_t secure_mode,
			  uint8_t trig_mode,
			  uint8_t priority,
			  uint8_t targets);

#endif /*__GIC_H */
