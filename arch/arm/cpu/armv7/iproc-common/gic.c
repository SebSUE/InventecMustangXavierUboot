/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <asm/iproc-common/gic.h>

struct _irq_handler {
	void                *m_data;
	void (*m_func)(void *data);
};

static struct _irq_handler IRQ_HANDLER[IPROC_NUM_CPUS][IPROC_NUM_IRQS];

static void default_isr(void *data)
{
	struct gic_data_struct *intdata_ptr = (struct gic_data_struct *)data;
	printf("default_isr:: IntrID=%0d, CPUID=%0d\n",
	       intdata_ptr->intid, intdata_ptr->cpuid);
}


unsigned int get_cpuid(void)
{
	unsigned int cpuid = 0;

	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 5" : "=r"(cpuid));

	cpuid &= 0xF;

	return cpuid;
}

void gic_enable_distributor(void)
{
	/* Supporting only secure mode for now */
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;
	dist_reg_ptr->control |= GIC_DIST_CTRL_S_EN_S;
}

void gic_disable_distributor(void)
{
	/* Supporting only secure mode for now */
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;
	dist_reg_ptr->control &= ~GIC_DIST_CTRL_S_EN_S;
}

/* Function to enable interrupt at distributor level */
void gic_enable_interrupt(uint8_t intid)
{
	uint8_t roffset, boffset;
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;

	roffset = intid / 32;
	boffset = intid % 32;
	dist_reg_ptr->iser[roffset] = (1 << boffset);
}

/* Function to disable interrupt at distributor level */
void gic_disable_interrupt(uint8_t intid)
{
	uint8_t roffset, boffset;
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;

	roffset = intid / 32;
	boffset = intid % 32;
	dist_reg_ptr->icer[roffset] = (1 << boffset);
}

/*
 * Set Software Generated Interrupt.
 * Parameters:
 * intid   - Interrupt ID (0 - 15)
 * filter  - Target filter, GIC_DIST_SGIR_TR_FILT_FW_LIST
 *			    GIC_DIST_SGIR_TR_FILT_FW_ALL_EX_ME
 *			    GIC_DIST_SGIR_TR_FILT_FW_ME_ONLY
 * targets - GIC_DIST_IPTR_CPU0 and/or GIC_DIST_IPTR_CPU1
 */
void gic_set_sgi(uint8_t intid, uint8_t filter, uint8_t targets)
{
	uint32_t sgir;
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;

	if (intid < 16) {
		sgir = intid;
		sgir |= (filter << GIC_DIST_SGIR_TR_FILT_BOFFSET) &
				GIC_DIST_SGIR_TR_FILT_MASK;
		if (filter == GIC_DIST_SGIR_TR_FILT_FW_LIST) {
			sgir |=
			(targets << GIC_DIST_SGIR_TR_LIST_BOFFSET) &
					GIC_DIST_SGIR_TR_LIST_MASK;
		}
		dist_reg_ptr->sgir = sgir;
	}
}

/*
 * Configure the interrupt properties in GIC distributor.
 * Parameters:
 * intid       - Interrupt ID (0 - IPROC_NUM_IRQS)
 * secure_mode - Secure mode/Non-Secure mode interrupt - TRUE = Secure mode int
 * trig_mode   - Level sensitive /Edge triggered
 *			(IPROC_INTR_LEVEL_SENSITIVE/IPROC_INTR_EDGE_TRIGGERED)
 *			Applicable only for SPIs.
 * priority    - ( 0 - 255)
 * targets     - CPU targets - GIC_DIST_IPTR_CPU0 and/or GIC_DIST_IPTR_CPU1
 */
void gic_config_interrupt(uint8_t intid,
			  uint8_t secure_mode,
			  uint8_t trig_mode,
			  uint8_t priority,
			  uint8_t targets)
{
	uint8_t roffset, boffset;
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;

	/* Disable this interrupt in distributor */
	roffset = intid / 32;
	boffset = intid % 32;
	dist_reg_ptr->icer[roffset] = (1 << boffset);

	if (secure_mode)
		dist_reg_ptr->isr[roffset] &= ~(1 << boffset);
	else
		dist_reg_ptr->isr[roffset] |= (1 << boffset);

	roffset = intid / 16;
	boffset = (intid % 16) * 2 + 1;

	if (trig_mode == IPROC_INTR_LEVEL_SENSITIVE)
		dist_reg_ptr->icfr[roffset] &= ~(1 << boffset);
	else if (trig_mode == IPROC_INTR_EDGE_TRIGGERED)
		dist_reg_ptr->icfr[roffset] |= (1 << boffset);

	roffset = intid / 4;
	boffset = (intid % 4) * 8;

	dist_reg_ptr->ipr[roffset] &= ~(0xFF << boffset);
	dist_reg_ptr->ipr[roffset] |= (priority << boffset);

	dist_reg_ptr->iptr[roffset] &= ~(0xFF << boffset);
	dist_reg_ptr->iptr[roffset] |= (targets << boffset);

	/* Enable this interrupt in distributor */
	roffset = intid / 32;
	boffset = intid % 32;
	dist_reg_ptr->iser[roffset] |= (1 << boffset);
}

static void gic_init(void)
{
	int i, num_reg, cpu_id;
	struct gic_dist_regs *dist_reg_ptr =
		(struct gic_dist_regs *)IPROC_PERIPH_INT_DISTR_REG_BASE;
	struct gic_ci_regs *ci_reg_ptr =
		(struct gic_ci_regs *)IPROC_PERIPH_INT_CTRL_REG_BASE;

	cpu_id = get_cpuid();

	if (cpu_id == 0) {
		dist_reg_ptr->control &= ~GIC_DIST_CTRL_S_EN_S;
		/* Initialize group registers */
		num_reg = IPROC_NUM_IRQS/32;
		for (i = 0; i < num_reg; i++) {
			/* All interrupts in secure mode */
			dist_reg_ptr->isr[i] = 0;
		}

		/* Initialize priority and target cpu registers */
		num_reg = IPROC_NUM_IRQS/4;
		for (i = 0; i < num_reg; i++) {
			dist_reg_ptr->ipr[i] = 0xFFFFFFFF;
			dist_reg_ptr->iptr[i] = 0;
		}
		/* Enable all inter sin the distributor */
		num_reg = IPROC_NUM_IRQS/32;
		for (i = 0; i < num_reg; i++)
			dist_reg_ptr->iser[i] = 0xFFFFFFFF;
	}

	/* Configure CPU interface registers */
	if (cpu_id == 0) {
		/* Set to lowest int priority, higher value is lower priority*/
		ci_reg_ptr->pmr = GIC_CI_PMR_PRIO_MASK;
	} else if (cpu_id == 1) {
		ci_reg_ptr->pmr = GIC_CI_PMR_PRIO_MASK;
	}

	/* Enable interrupts at CPU interface level */
	ci_reg_ptr->control = GIC_CI_CTRL_EN;

	/* Enable distributor */
	if (cpu_id == 0)
		dist_reg_ptr->control |= GIC_DIST_CTRL_S_EN_S;
}

#ifdef CONFIG_USE_IRQ
void do_irq(struct pt_regs *pt_regs)
{
	struct gic_data_struct intdata;
	int cur_cpuid;
	uint32_t iar;

	struct gic_ci_regs *ci_reg_ptr =
		(struct gic_ci_regs *)IPROC_PERIPH_INT_CTRL_REG_BASE;

	cur_cpuid = get_cpuid();
	iar = ci_reg_ptr->iar;
	intdata.cpuid = (iar & GIC_CI_IAR_CPUID_MASK) >> 10;
	intdata.intid = iar & GIC_CI_IAR_INTID_MASK;

	if (cur_cpuid == 0) {
		intdata.data = (void *)IRQ_HANDLER[0][intdata.intid].m_data;
		IRQ_HANDLER[0][intdata.intid].m_func((void *)&intdata);
	} else if (cur_cpuid == 1) {
		intdata.data = (void *)IRQ_HANDLER[1][intdata.intid].m_data;
		IRQ_HANDLER[1][intdata.intid].m_func((void *)&intdata);
	}

	/* Acknowledge the completion of interrupt processing */
	if (intdata.intid < 16)
		ci_reg_ptr->eoir |= intdata.cpuid;

	ci_reg_ptr->eoir |= intdata.intid;
}
#endif

void irq_install_handler(int irq,
			 interrupt_handler_t handle_irq,
			 void *data)
{
	int cur_cpuid;
	if (irq >= IPROC_NUM_IRQS || !handle_irq)
		return;
	cur_cpuid = get_cpuid();
	if (cur_cpuid == 0) {
		IRQ_HANDLER[0][irq].m_data = data;
		IRQ_HANDLER[0][irq].m_func = handle_irq;
	} else if (cur_cpuid == 1) {
		IRQ_HANDLER[1][irq].m_data = data;
		IRQ_HANDLER[1][irq].m_func = handle_irq;
	}
}

int arch_interrupt_init(void)
{
	int i, j;

	/* install default interrupt handlers */
	for (i = 0; i < IPROC_NUM_CPUS; i++) {
		for (j = 0; j < IPROC_NUM_IRQS; j++) {
			IRQ_HANDLER[i][j].m_data = NULL;
			IRQ_HANDLER[i][j].m_func =
				(interrupt_handler_t *)default_isr;
		}
	}

	/* Intialize the GIC */
	gic_init();

	return 0;
}
