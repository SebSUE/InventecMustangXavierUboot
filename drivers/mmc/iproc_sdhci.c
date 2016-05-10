/*
 * Copyright 2013 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/errno.h>

struct sdhci_iproc_host {
	struct sdhci_host host;
	u32 shadow_cmd;
	u32 shadow_blk;
};

#define REG_OFFSET_IN_BITS(reg) ((reg) << 3 & 0x18)

static inline struct sdhci_iproc_host *to_iproc(struct sdhci_host *host)
{
	return (struct sdhci_iproc_host *)host;
}

static u32 sdhci_iproc_readl(struct sdhci_host *host, int reg)
{
	u32 val = readl(host->ioaddr + reg);
#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS_TRACE
	printf("%s %d: readl [0x%02x] 0x%08x\n",
	       host->name, host->index, reg, val);
#endif
	return val;
}

static u16 sdhci_iproc_readw(struct sdhci_host *host, int reg)
{
	u32 val = sdhci_iproc_readl(host, (reg & ~3));
	u16 word = val >> REG_OFFSET_IN_BITS(reg) & 0xffff;
	return word;
}

static u8 sdhci_iproc_readb(struct sdhci_host *host, int reg)
{
	u32 val = sdhci_iproc_readl(host, (reg & ~3));
	u8 byte = val >> REG_OFFSET_IN_BITS(reg) & 0xff;
	return byte;
}

static void sdhci_iproc_writel(struct sdhci_host *host, u32 val, int reg)
{
	u32 clock = 0;
#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS_TRACE
	printf("%s %d: writel [0x%02x] 0x%08x\n",
	       host->name, host->index, reg, val);
#endif
	writel(val, host->ioaddr + reg);

	if (host->mmc)
		clock = host->mmc->clock;
	if (clock <= 400000) {
		/* Round up to micro-second four SD clock delay */
		if (clock)
			udelay((4 * 1000000 + clock - 1) / clock);
		else
			udelay(10);
	}
}

/*
 * The Arasan has a bugette whereby it may lose the content of successive
 * writes to the same register that are within two SD-card clock cycles of
 * each other (a clock domain crossing problem). The data
 * register does not have this problem, which is just as well - otherwise we'd
 * have to nobble the DMA engine too.
 *
 * This wouldn't be a problem with the code except that we can only write the
 * controller with 32-bit writes.  So two different 16-bit registers are
 * written back to back creates the problem.
 *
 * In reality, this only happens when SDHCI_BLOCK_SIZE and SDHCI_BLOCK_COUNT
 * are written followed by SDHCI_TRANSFER_MODE and SDHCI_COMMAND.
 * The BLOCK_SIZE and BLOCK_COUNT are meaningless until a command issued so
 * the work around can be further optimized. We can keep shadow values of
 * BLOCK_SIZE, BLOCK_COUNT, and TRANSFER_MODE until a COMMAND is issued.
 * Then, write the BLOCK_SIZE+BLOCK_COUNT in a single 32-bit write followed
 * by the TRANSFER+COMMAND in another 32-bit write.
 */
static void sdhci_iproc_writew(struct sdhci_host *host, u16 val, int reg)
{
	struct sdhci_iproc_host *iproc_host = to_iproc(host);
	u32 word_shift = REG_OFFSET_IN_BITS(reg);
	u32 mask = 0xffff << word_shift;
	u32 oldval, newval;

	if (reg == SDHCI_COMMAND) {
		/* Write the block now as we are issuing a command */
		if (iproc_host->shadow_blk != 0) {
			sdhci_iproc_writel(host, iproc_host->shadow_blk,
					   SDHCI_BLOCK_SIZE);
			iproc_host->shadow_blk = 0;
		}
		oldval = iproc_host->shadow_cmd;
	} else if (reg == SDHCI_BLOCK_SIZE || reg == SDHCI_BLOCK_COUNT) {
		/* Block size and count are stored in shadow reg */
		oldval = iproc_host->shadow_blk;
	} else {
		/* Read reg, all other registers are not shadowed */
		oldval = sdhci_iproc_readl(host, (reg & ~3));
	}
	newval = (oldval & ~mask) | (val << word_shift);

	if (reg == SDHCI_TRANSFER_MODE) {
		/* Save the transfer mode until the command is issued */
		iproc_host->shadow_cmd = newval;
	} else if (reg == SDHCI_BLOCK_SIZE || reg == SDHCI_BLOCK_COUNT) {
		/* Save the block info until the command is issued */
		iproc_host->shadow_blk = newval;
	} else {
		/* Command or other regular 32-bit write */
		sdhci_iproc_writel(host, newval, reg & ~3);
	}
}

static void sdhci_iproc_writeb(struct sdhci_host *host, u8 val, int reg)
{
	u32 oldval = sdhci_iproc_readl(host, (reg & ~3));
	u32 byte_shift = REG_OFFSET_IN_BITS(reg);
	u32 mask = 0xff << byte_shift;
	u32 newval = (oldval & ~mask) | (val << byte_shift);

	sdhci_iproc_writel(host, newval, reg & ~3);
}

static struct sdhci_ops sdhci_platform_ops = {
#ifdef CONFIG_MMC_SDHCI_IO_ACCESSORS
	.read_l = sdhci_iproc_readl,
	.read_w = sdhci_iproc_readw,
	.read_b = sdhci_iproc_readb,
	.write_l = sdhci_iproc_writel,
	.write_w = sdhci_iproc_writew,
	.write_b = sdhci_iproc_writeb,
#else
#error The iproc SDHCI driver needs CONFIG_MMC_SDHCI_IO_ACCESSORS to be set
#endif
};

int iproc_sdhci_init(int dev_index, u32 quirks)
{
	struct sdhci_iproc_host *iproc_host;
	int ret = 0;
	void *reg_base;
	struct sdhci_host *host = NULL;

	iproc_host = (struct sdhci_iproc_host *)
			malloc(sizeof(struct sdhci_iproc_host));
	if (!iproc_host) {
		printf("%s: sdhci host malloc fail!\n", __func__);
		return -ENOMEM;
	}
	switch (dev_index) {
	case 0:
		reg_base = (void *)CONFIG_SYS_SDIO0_BASE;
		break;
	case 1:
		reg_base = (void *)CONFIG_SYS_SDIO1_BASE;
		break;
	default:
		printf("%s: sdio dev index %d not supported\n",
		       __func__, dev_index);
		ret = -EINVAL;
	}
	if (ret)
		return ret;

	iproc_host->shadow_cmd = 0;
	iproc_host->shadow_blk = 0;

	host = &iproc_host->host;
	host->name = "iproc-sdhci";
	host->ioaddr = reg_base;
	host->voltages = MMC_VDD_165_195 |
			 MMC_VDD_32_33 | MMC_VDD_33_34;
	host->quirks = SDHCI_QUIRK_BROKEN_VOLTAGE;
	host->quirks |= quirks;
	host->host_caps = 0;
	host->index = dev_index;
	host->ops = &sdhci_platform_ops;

	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);

	ret = add_sdhci(host, CONFIG_SYS_SDIO_MAX_CLK, CONFIG_SYS_SDIO_MIN_CLK);

	return ret;
}
