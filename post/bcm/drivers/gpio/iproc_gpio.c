/*
 * $Copyright Open Broadcom Corporation$
 */

#include <common.h>
#include <asm/io.h>
#include "iproc_gpio.h"

#if defined(CONFIG_CYGNUS)

#define IPROC_GIC_DIST_IPTR_CPU0 0x01
#define IPROC_INTR_LEVEL_SENSITIVE 1
#define IPROC_INTR_EDGE_TRIGGERED 2

#define ChipcommonG_GP_INT_MSK 0x1800a018
#define ChipcommonG_GP_INT_MSK_BASE 0x018
#define ChipcommonG_GP_INT_TYPE 0x1800a00c
#define ChipcommonG_GP_INT_TYPE_BASE 0x00c
#define ChipcommonG_GP_INT_STAT 0x1800a01c
#define ChipcommonG_GP_INT_STAT_BASE 0x01c
#define ChipcommonG_GP_DATA_IN 0x1800a000
#define ChipcommonG_GP_DATA_IN_BASE 0x000
#define ChipcommonG_GP_INT_CLR 0x1800a024
#define ChipcommonG_GP_INT_CLR_BASE 0x024
#define ChipcommonG_GP_INT_EDGE 0x1800a014
#define ChipcommonG_GP_INT_EDGE_BASE 0x014
#define ChipcommonG_GP_INT_DE 0x1800a010
#define ChipcommonG_GP_INT_DE_BASE 0x010

#if 0
#define IPROC_GPIO_CCA_INT_EVENT_MASK  ChipcommonG_GP_INT_MSK_BASE
#define IPROC_GPIO_CCA_INT_LEVEL_MASK  ChipcommonG_GP_INT_MSK_BASE
#define IPROC_GPIO_CCA_INT_EVENT       ChipcommonG_GP_INT_STAT_BASE
#define IPROC_GPIO_CCA_INT_LEVEL       ChipcommonG_GP_INT_TYPE_BASE
#define IPROC_GPIO_CCA_INT_EDGE        ChipcommonG_GP_INT_TYPE_BASE
#define IPROC_GPIO_CCA_DIN             ChipcommonG_GP_DATA_IN_BASE
#endif

#endif

extern int countLevelHigh;
extern int countLevelLow;
extern int countRisingEdge;
extern int countFalliingEdge;
extern int numberOfGPIOs;

extern int receivedINT;

int IntReceived = 0;

static iproc_gpio_chip gpioDev;
static int currentIRQType;

static unsigned int _iproc_gpio_readl(iproc_gpio_chip *chip, int reg)
{
	return readl(chip->ioaddr + reg);
}

static void _iproc_gpio_writel(iproc_gpio_chip *chip, unsigned int val, int reg)
{
	writew(val, chip->ioaddr + reg);
}


static int iproc_gpio_to_irq(iproc_gpio_chip *chip, unsigned int pin) {
	return chip->irq_base + pin;
}

static int iproc_irq_to_gpio(iproc_gpio_chip *chip, unsigned int irq) {
	return irq - chip->irq_base;
}


static void iproc_gpio_irq_unmask(unsigned int irq, IRQTYPE irq_type)
{
	iproc_gpio_chip *ourchip = &gpioDev;
	int pin;
	unsigned int int_mask;

	pin = iproc_irq_to_gpio(ourchip, irq);
	/*irq_type = IRQ_TYPE_EDGE_BOTH;//for now swang //irq_desc[irq].status & IRQ_TYPE_SENSE_MASK;*/

	if ((ourchip->id == IPROC_GPIO_CCA_ID) ||
		(ourchip->id == IPROC_GPIO_CCG_ID)) {
		unsigned int  event_mask;

		event_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT_MASK);
		int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL_MASK);

		if (irq_type & IRQ_TYPE_EDGE_BOTH) {
			event_mask |= 1 << pin;
			_iproc_gpio_writel(ourchip, event_mask,
				IPROC_GPIO_CCA_INT_EVENT_MASK);
		} else {
			int_mask |= 1 << pin;
			_iproc_gpio_writel(ourchip, int_mask,
				IPROC_GPIO_CCA_INT_LEVEL_MASK);
		}
	}

}
static void iproc_gpio_irq_mask(unsigned int irq, IRQTYPE irq_type)
{
	iproc_gpio_chip *ourchip = &gpioDev;
	int pin;
	unsigned int int_mask;


	pin = iproc_irq_to_gpio(ourchip, irq);
	/*irq_type = IRQ_TYPE_EDGE_BOTH; //for now swang ???//irq_desc[irq].status & IRQ_TYPE_SENSE_MASK;*/

	if ((ourchip->id == IPROC_GPIO_CCA_ID) ||
		(ourchip->id == IPROC_GPIO_CCG_ID)) {
		unsigned int  event_mask;

		event_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT_MASK);
		int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL_MASK);

		if (irq_type & IRQ_TYPE_EDGE_BOTH) {
			event_mask &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_mask,
				IPROC_GPIO_CCA_INT_EVENT_MASK);
		} else {
			int_mask &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, int_mask,
				IPROC_GPIO_CCA_INT_LEVEL_MASK);
		}
	}
}

static int iproc_gpio_irq_set_type(unsigned int irq, IRQTYPE type)
{
	iproc_gpio_chip *ourchip = &gpioDev;
	int pin;
	unsigned int  event_pol, int_pol;

	currentIRQType = type;

	pin = iproc_irq_to_gpio(ourchip, irq);

#if defined(CONFIG_CYGNUS)
	if (ourchip->id == IPROC_GPIO_CCG_ID) {
		switch (type) {
		case IRQ_TYPE_EDGE_RISING:
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_TYPE_BASE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_TYPE_BASE);
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_DE_BASE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_DE_BASE);
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_EDGE_BASE);
			event_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_EDGE_BASE);
			break;
		case IRQ_TYPE_EDGE_FALLING:
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_TYPE_BASE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_TYPE_BASE);
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_DE_BASE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_DE_BASE);
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_EDGE_BASE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_EDGE_BASE);
			break;
		case IRQ_TYPE_LEVEL_HIGH:
			int_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_TYPE_BASE);
			int_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, ChipcommonG_GP_INT_TYPE_BASE);
			int_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_DE_BASE);
			int_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, ChipcommonG_GP_INT_DE_BASE);
			int_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_EDGE_BASE);
			int_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, ChipcommonG_GP_INT_EDGE_BASE);
			break;
		case IRQ_TYPE_LEVEL_LOW:
			int_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_TYPE_BASE);
			int_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, ChipcommonG_GP_INT_TYPE_BASE);
			int_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_DE_BASE);
			int_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, ChipcommonG_GP_INT_DE_BASE);
			int_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_EDGE_BASE);
			int_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, ChipcommonG_GP_INT_EDGE_BASE);
			break;
		case IRQ_TYPE_EDGE_BOTH:
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_TYPE_BASE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_TYPE_BASE);
			event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_DE_BASE);
			event_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, ChipcommonG_GP_INT_DE_BASE);
			break;
		default:
			/*printf(KERN_ERR "unsupport irq type !\n");*/
			return -1;
		}
	}
#else
	if (ourchip->id == IPROC_GPIO_CCA_ID) {
		switch (type) {
		case IRQ_TYPE_EDGE_RISING:
			event_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EDGE);
			event_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, IPROC_GPIO_CCA_INT_EDGE);
			break;
		case IRQ_TYPE_EDGE_FALLING:
			event_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EDGE);
			event_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, event_pol, IPROC_GPIO_CCA_INT_EDGE);
			break;
		case IRQ_TYPE_LEVEL_HIGH:
			int_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL);
			int_pol &= ~(1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, IPROC_GPIO_CCA_INT_LEVEL);
			break;
		case IRQ_TYPE_LEVEL_LOW:
			int_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL);
			int_pol |= (1 << pin);
			_iproc_gpio_writel(ourchip, int_pol, IPROC_GPIO_CCA_INT_LEVEL);
			break;
		default:
			/*printf(KERN_ERR "unsupport irq type !\n");*/
			return -1;
		}
	}
#endif
	return 0;
}

extern void iproc_aon_gpio_irq_handler(void *data);
void iproc_gpio_irq_handler(void *data)
{
	unsigned int  event_pol, in, int_mask;
	unsigned int  event_mask, event, tmp = 0;
	int iter;
	iproc_gpio_chip *ourchip = &gpioDev;

#if defined(CONFIG_CYGNUS)

	iproc_aon_gpio_irq_handler(data);

	event_mask = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_MSK_BASE);
	event_pol = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_TYPE_BASE);
	in = _iproc_gpio_readl(ourchip, ChipcommonG_GP_DATA_IN_BASE);
	event = _iproc_gpio_readl(ourchip, ChipcommonG_GP_INT_STAT_BASE);

	tmp = event_mask & event;
	/*24/32is the number of GPIOs*/
	for (iter = 0; iter < numberOfGPIOs; iter++) {
		if (tmp & (1 << iter)) {
			if (tmp  & (1 << iter) & (~event_pol)) {/*edge triggered*/
				if (in & (1 << iter)) {
					countRisingEdge |= 1 << iter;
					puts("\nRising edge interrupt");
				} else {
					countFalliingEdge |= 1 << iter;
					puts("\nFalling edge interrupt");
				}
				receivedINT = iter;
			} else {
				if (in & (1 << iter)) {
					countLevelHigh |= 1 << iter;
					puts("\nLevel high interrupt");
				} else {
					countLevelLow |= 1 << iter;
					puts("\nLevel low interrupt");
				}
			}
			/* clear the interrupt*/
			int_mask = readl(ChipcommonG_GP_INT_CLR);
			writel(int_mask | (1 << iter), ChipcommonG_GP_INT_CLR);
			IntReceived = 1;
		}
	}

#else
	unsigned int  val, int_mask, int_pol;

	val = readl(ourchip->intr_ioaddr + CCA_INT_STS_BASE);

	if (val & IPROC_CCA_INT_F_GPIOINT) {
		/*Disable GPIO interrupt at top level*/
		tmp = readl(gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
		writew((tmp & ~IPROC_CCA_INT_F_GPIOINT), gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);

		int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL_MASK);
		int_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL);
		in = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_DIN);
		event_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT_MASK);
		event = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT);
		event_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EDGE);

		/*24/32is the number of GPIOs*/
		for (iter = 0; iter < numberOfGPIOs; iter++) {
			if ((IRQ_TYPE_EDGE_FALLING == currentIRQType) ||
				(IRQ_TYPE_EDGE_RISING == currentIRQType)) {
				tmp = event_mask;
				tmp &= event;
				/* If there is an event interrupt ( edge triggered )*/
				if (tmp & (1 << iter)) {
					if ((~event_pol)&(1 << iter)) {
						countRisingEdge |= 1 << iter;
						puts("\nRising edge interrupt");
						receivedINT = iter;
					} else {
						countFalliingEdge |= 1 << iter;
						puts("\nFalling edge interrupt");
						receivedINT = iter;
					}
					IntReceived = 1;
					/*Clear the edge interrupt*/
					tmp = readl(CCA_GPIO_EVT);
					writel(tmp | (1 << iter), CCA_GPIO_EVT);
					tmp = readl(CCA_GPIO_EVTINT_MASK);
					writel(tmp & ~(1 << iter), CCA_GPIO_EVTINT_MASK);
				}
			} else {
				tmp = in ^ int_pol;
				tmp &= int_mask;

				if (tmp & (1 << iter)) {
					if (int_pol & (1 << iter)) {
						countLevelLow |= 1 << iter;
						/*puts("\nLevel low interrupt");*/
					} else {
						countLevelHigh |= 1 << iter;
						/*puts("\nLevel high interrupt");*/
					}
					tmp = readl(CCA_GPIOINT_MASK);
					writel(tmp & ~(1 << iter), CCA_GPIOINT_MASK);
					IntReceived = 1;
				}
			}
		}
		/*Enable GPIO interrupt at top level*/
		tmp = readl(gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
		writew((tmp | IPROC_CCA_INT_F_GPIOINT), gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
	}
#endif

	return;
}

/*set a GPIO pin as an input*/
static int iproc_gpiolib_input(iproc_gpio_chip *ourchip, unsigned gpio)
{
	unsigned int  val;
	unsigned int  nBitMask = 1 << gpio;

	/*iproc_gpio_lock(ourchip, flags);*/

	val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
	val &= ~nBitMask;
	_iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_EN);

	/*iproc_gpio_unlock(ourchip, flags);*/
	return 0;
}

//set a gpio pin as a output
static int iproc_gpiolib_output(iproc_gpio_chip *ourchip,
				  unsigned gpio, int value)
{
	unsigned long val;
	unsigned int  nBitMask = 1 << gpio;

	/*iproc_gpio_lock(ourchip, flags);*/

	val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
	val |= nBitMask;
	_iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_EN);

	/*iproc_gpio_unlock(ourchip, flags);*/
	return 0;
}

/*set the gpio pin ( by gpio ) to value ( only for output, input is read only*/
static void iproc_gpiolib_set(iproc_gpio_chip *ourchip,
				unsigned gpio, int value)
{
	unsigned long val;
	unsigned int  nBitMask = 1 << gpio;

	/*iproc_gpio_lock(ourchip, flags);*/


	/* determine the GPIO pin direction
	 */
	val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
	val &= nBitMask;

	/* this function only applies to output pin
	 */
	if (!val)
		return;

	val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DOUT);

	if (value == 0)
		/* Set the pin to zero */
		val &= ~nBitMask;
	else
		/* Set the pin to 1 */
		val |= nBitMask;

	_iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_DOUT);

	/*iproc_gpio_unlock(ourchip, flags);*/

}

/*get the gpio pin value*/
static int iproc_gpiolib_get(iproc_gpio_chip *ourchip, unsigned gpio)
{
	unsigned int val, offset;
	unsigned int  nBitMask = 1 << gpio;

	/*iproc_gpio_lock(ourchip, flags);*/
	/* determine the GPIO pin direction
	 */
	offset = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
	offset &= nBitMask;

	/*output pin*/
	if (offset)
		val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DOUT);
	/*input pin*/
	else
		val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DIN);

	val >>= gpio;
	val &= 1;
	/*printf("\nGPIO %d, input value reg = 0x%x,,value= %d ,offset %d", gpio, *((unsigned long*)0x18000060),val, offset);
	iproc_gpio_unlock(ourchip, flags);*/

	return val;
}
#if 0
static int iproc_gpiolib_to_irq(iproc_gpio_chip *ourchip,
				unsigned offset)
{
	return iproc_gpio_to_irq(ourchip, offset);
}
#endif

void iprocGPIOInit( void )
{
#if defined(CONFIG_CYGNUS)
	gpioDev.id = IPROC_GPIO_CCG_ID;
	gpioDev.ioaddr = (void *)ChipcommonG_GP_DATA_IN;/*IPROC_GPIO_CCA_BASE;*/
	gpioDev.dmu_ioaddr = 0;/*(void*)NORTHSTAR_DMU_BASE;*/
	gpioDev.intr_ioaddr = (void *)ChipcommonG_GP_DATA_IN;/*CCA_CHIPID;*/
	gpioDev.irq_base = 0;
#else
	gpioDev.id = IPROC_GPIO_CCA_ID;
	gpioDev.ioaddr = (void *)IPROC_GPIO_CCA_BASE;
	gpioDev.dmu_ioaddr = (void *)NORTHSTAR_DMU_BASE;
	gpioDev.intr_ioaddr = (void *)CCA_CHIPID ;
	gpioDev.irq_base = IPROC_GPIO_CCA_IRQ_BASE;
#endif
}

iproc_gpio_chip *getGPIODev(void)
{
	return &gpioDev;
}

void setGPIOasInput(int pin)
{
	if ((pin < numberOfGPIOs) && (pin >= 0))
		iproc_gpiolib_input(&gpioDev, pin);
}

void setGPIOasOutput(int pin, int value)
{
	if ((pin < numberOfGPIOs) && (pin >= 0)) {
		/*Set the pin to be output*/
		iproc_gpiolib_output(&gpioDev, pin, value);
		/*set value*/
		iproc_gpiolib_set(&gpioDev, pin, value);
	}
}

int getGPIOpinValue(int pin)
{
	if ((pin < numberOfGPIOs) && (pin >= 0))
		return iproc_gpiolib_get(&gpioDev, pin);
	return -1;
}

void enableGPIOInterrupt(int pin, IRQTYPE irqtype)
{
	unsigned int irq;
	unsigned long temp;

#if 0
	unsigned long tempDelay = 0;
	if (readl(0x18000020) & 0x1)
		post_log("\nInterrupt still on!!!");
	else
		post_log("\nInterrupt not on!!!");
#endif
	if ((pin < numberOfGPIOs) && (pin >= 0)) {
		/*Map pin to IRQ number*/
		irq = iproc_gpio_to_irq(&gpioDev, pin);
		iproc_gpio_irq_set_type(irq, irqtype);
		iproc_gpio_irq_unmask(irq, irqtype);

		/*while ( tempDelay < 0xFFFF )
		{
		    tempDelay++;
		}
		post_log("\n Enable Interrupt");*/
		/*Enable GPIO interrupt at top level*/
		temp = readl(gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
		writew((temp | IPROC_CCA_INT_F_GPIOINT), gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
	}
}

void disableGPIOInterrupt(int pin, IRQTYPE irqtype)
{
	unsigned int irq;
	unsigned long temp;

	if ((pin < numberOfGPIOs) && (pin >= 0)) {
		/*Map pin to IRQ number*/
		irq = iproc_gpio_to_irq(&gpioDev, pin);
		/*iproc_gpio_irq_set_type(irq,irqtype);*/
		iproc_gpio_irq_mask(irq, irqtype);

		/*Disable GPIO interrupt at top level*/
		temp = readl(gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
		writew((temp & ~IPROC_CCA_INT_F_GPIOINT), gpioDev.intr_ioaddr + CCA_INT_MASK_BASE);
	}
}

extern void iproc_gpio_iomux(int op);
void initGPIOState(void)
{
#if defined(CONFIG_CYGNUS)
#ifndef CONFIG_CYGNUS_EMULATION
	iproc_gpio_iomux(1);
#endif
#endif
	_iproc_gpio_writel(&gpioDev, 0xFFFFFF, IPROC_GPIO_CCA_INT_EVENT);
	_iproc_gpio_writel(&gpioDev, 0x000000, IPROC_GPIO_CCA_INT_LEVEL);
	_iproc_gpio_writel(&gpioDev, 0x000000, IPROC_GPIO_CCA_INT_LEVEL_MASK);
	_iproc_gpio_writel(&gpioDev, 0x000000, IPROC_GPIO_CCA_INT_EVENT_MASK);
	_iproc_gpio_writel(&gpioDev, 0x000000, IPROC_GPIO_CCA_INT_EDGE);
}

extern void gic_config_interrupt(uint8_t intid, uint8_t secure_mode, uint8_t trig_mode, uint8_t priority, uint8_t targets);
extern void gic_disable_interrupt(uint8_t intid);
void registerGPIOEdgeInterrupt(void)
{
	disable_interrupts();
	gic_disable_interrupt(117);
	irq_install_handler(117, iproc_gpio_irq_handler, NULL);
	gic_config_interrupt(117, 1, IPROC_INTR_EDGE_TRIGGERED, 0, IPROC_GIC_DIST_IPTR_CPU0);
	enable_interrupts();
}

void registerGPIOLevelInterrupt(void)
{
	disable_interrupts();
	gic_disable_interrupt(117);
	irq_install_handler(117, iproc_gpio_irq_handler, NULL);
	gic_config_interrupt(117, 1, IPROC_INTR_LEVEL_SENSITIVE, 0, IPROC_GIC_DIST_IPTR_CPU0);
	enable_interrupts();
}
