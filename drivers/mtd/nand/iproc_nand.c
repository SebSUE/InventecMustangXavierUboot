/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <asm/errno.h>
#include <asm/io.h>
#include <asm-generic/unaligned.h>

#include <config.h>
#include <common.h>
#include <nand.h>
#include <malloc.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#include "iproc_nand.h"

/***********************************************************************
 * Definitions
 ***********************************************************************/
#define DRV_NAME                        "iproc_nand"

/*
 * iProc NAND flash commands
 */
#define CMD_PAGE_READ                   0x01
#define CMD_SPARE_AREA_READ             0x02
#define CMD_STATUS_READ                 0x03
#define CMD_PROGRAM_PAGE                0x04
#define CMD_PROGRAM_SPARE_AREA          0x05
#define CMD_COPY_BACK                   0x06
#define CMD_DEVICE_ID_READ              0x07
#define CMD_BLOCK_ERASE                 0x08
#define CMD_FLASH_RESET                 0x09
#define CMD_BLOCKS_LOCK                 0x0a
#define CMD_BLOCKS_LOCK_DOWN            0x0b
#define CMD_BLOCKS_UNLOCK               0x0c
#define CMD_READ_BLOCKS_LOCK_STATUS     0x0d
#define CMD_PARAMETER_READ              0x0e
#define CMD_PARAMETER_CHANGE_COL        0x0f
#define CMD_LOW_LEVEL_OP                0x10
#define CMD_PAGE_READ_MULTI		0x11
#define CMD_STATUS_READ_MULTI		0x12
#define CMD_PROGRAM_PAGE_MULTI		0x13
#define CMD_PROGRAM_PAGE_MULTI_CACHE	0x14
#define CMD_BLOCK_ERASE_MULTI		0x15

/*
 * NAND controller register offset
 */
#define NCREG_REVISION                 0x000	/* Revision */
#define NCREG_CMD_START                0x004	/* Command Start */
#define NCREG_CMD_EXT_ADDRESS          0x008	/* Command Extended Address */
#define NCREG_CMD_ADDRESS              0x00c	/* Command Address */
#define NCREG_CMD_END_ADDRESS          0x010	/* Command End Address */
#define NCREG_INTFC_STATUS             0x014	/* Interface Status */
#define NCREG_CS_NAND_SELECT           0x018	/* EBI CS Select */
#define NCREG_CS_NAND_XOR              0x01c	/* EBI CS Address XOR with
						   1FC0 Control */
#define NCREG_LL_OP                    0x020	/* Low Level Operation */
#define NCREG_MPLANE_BASE_EXT_ADDRESS  0x024	/* Multiplane base extended
						   address */
#define NCREG_MPLANE_BASE_ADDRESS      0x028	/* Multiplane base address */
#define NCREG_ACC_CONTROL_CS0          0x050	/* Access Control CS0 */
#define NCREG_CONFIG_CS0               0x054	/* Config CS0 */
#define NCREG_TIMING_1_CS0             0x058	/* Timing Parameters 1 CS0 */
#define NCREG_TIMING_2_CS0             0x05c	/* Timing Parameters 2 CS0 */
#define NCREG_ACC_CONTROL_CS1          0x060	/* Access Control CS1 */
#define NCREG_CONFIG_CS1               0x064	/* Config CS1 */
#define NCREG_TIMING_1_CS1             0x068	/* Timing Parameters 1 CS1 */
#define NCREG_TIMING_2_CS1             0x06c	/* Timing Parameters 2 CS1 */
#define NCREG_CORR_STAT_THRESHOLD      0x0c0	/* Correctable Error Reporting
						   Threshold */
#define NCREG_BLK_WR_PROTECT           0x0c8	/* Block Write Protect Enable
						   and Size for EBI_CS0b */
#define NCREG_MULTIPLANE_OPCODES_1     0x0cc	/* Multiplane Customized
						   Opcodes */
#define NCREG_MULTIPLANE_OPCODES_2     0x0d0	/* Multiplane Customized
						   Opcodes */
#define NCREG_MULTIPLANE_CTRL          0x0d4	/* Multiplane Control */
#define NCREG_UNCORR_ERROR_COUNT       0x0fc	/* Total Uncorrectable Error
						   Count */
#define NCREG_CORR_ERROR_COUNT         0x100	/* Correctable Error Count */
#define NCREG_READ_ERROR_COUNT         0x104	/* Total Correctable Error
						   Count */
#define NCREG_BLOCK_LOCK_STATUS        0x108	/* Block Lock Status */
#define NCREG_ECC_CORR_EXT_ADDR        0x10c	/* ECC Correctable Error
						   Extended Address */
#define NCREG_ECC_CORR_ADDR            0x110	/* ECC Correctable Error
						   Address */
#define NCREG_ECC_UNC_EXT_ADDR         0x114	/* ECC Uncorrectable Error
						   Extended Address */
#define NCREG_ECC_UNC_ADDR             0x118	/* ECC Uncorrectable Error
						   Address */
#define NCREG_FLASH_READ_EXT_ADDR      0x11c	/* Read Data Extended Address */
#define NCREG_FLASH_READ_ADDR          0x120	/* Read Data Address */
#define NCREG_PROGRAM_PAGE_EXT_ADDR    0x124	/* Page Program Extended
						   Address */
#define NCREG_PROGRAM_PAGE_ADDR        0x128	/* Page Program Address */
#define NCREG_COPY_BACK_EXT_ADDR       0x12c	/* Copy Back Extended Address */
#define NCREG_COPY_BACK_ADDR           0x130	/* Copy Back Address */
#define NCREG_BLOCK_ERASE_EXT_ADDR     0x134	/* Block Erase Extended
						   Address */
#define NCREG_BLOCK_ERASE_ADDR         0x138	/* Block Erase Address */
#define NCREG_INV_READ_EXT_ADDR        0x13c	/* Invalid Data Extended
						   Address */
#define NCREG_INV_READ_ADDR            0x140	/* Invalid Data Address */
#define NCREG_INIT_STATUS              0x144	/* Initialization status */
#define NCREG_ONFI_STATUS              0x148	/* ONFI Status */
#define NCREG_ONFI_DEBUG_DATA          0x14c	/* ONFI Debug Data */
#define NCREG_SEMAPHORE                0x150	/* Semaphore */
#define NCREG_FLASH_DEVICE_ID          0x194	/* Device ID */
#define NCREG_FLASH_DEVICE_ID_EXT      0x198	/* Extended Device ID */
#define NCREG_LL_RDDATA                0x19c	/* Low Level Read Data */
#define NCREG_SPARE_AREA_READ_OFS_0    0x200	/* Spare Area Read Bytes */
#define NCREG_SPARE_AREA_WRITE_OFS_0   0x280	/* Spare Area Write Bytes */
#define NCREG_FLASH_CACHE_BASE         0x400	/* Cache Buffer Access */
#define NCREG_INTERRUPT_BASE           0xf00	/* Interrupt Base Address */

/*
 * Required NAND controller register fields
 */
#define NCFLD_CMD_START_OPCODE_SHIFT                            24
#define NCFLD_INTFC_STATUS_FLASH_STATUS_MASK                    0x000000FF
#define NCFLD_CS_NAND_SELECT_AUTO_DEVID_CONFIG                  0x40000000
#define NCFLD_CS_NAND_SELECT_WP                                 0x20000000
#define NCFLD_CS_NAND_SELECT_DIRECT_ACCESS_CS_MASK              0x000000FF
#define NCFLD_CS_NAND_XOR_CS_MASK                               0x000000FF
#define NCFLD_CONFIG_CS0_BLOCK_SIZE_MASK                        0x70000000
#define NCFLD_CONFIG_CS0_BLOCK_SIZE_SHIFT                       28
#define NCFLD_CONFIG_CS0_DEVICE_SIZE_MASK                       0x0f000000
#define NCFLD_CONFIG_CS0_DEVICE_SIZE_SHIFT                      24
#define NCFLD_CONFIG_CS0_DEVICE_WIDTH_MASK                      0x00800000
#define NCFLD_CONFIG_CS0_DEVICE_WIDTH_SHIFT                     23
#define NCFLD_CONFIG_CS0_PAGE_SIZE_MASK                         0x00300000
#define NCFLD_CONFIG_CS0_PAGE_SIZE_SHIFT                        20
#define NCFLD_CONFIG_CS0_FUL_ADR_BYTES_MASK                     0x00070000
#define NCFLD_CONFIG_CS0_FUL_ADR_BYTES_SHIFT                    16
#define NCFLD_CONFIG_CS0_COL_ADR_BYTES_MASK                     0x00007000
#define NCFLD_CONFIG_CS0_COL_ADR_BYTES_SHIFT                    12
#define NCFLD_CONFIG_CS0_BLK_ADR_BYTES_MASK                     0x00000700
#define NCFLD_CONFIG_CS0_BLK_ADR_BYTES_SHIFT                    8
#define NCFLD_ACC_CONTROL_CS0_RD_ECC_EN_MASK                    0x80000000
#define NCFLD_ACC_CONTROL_CS0_RD_ECC_EN_SHIFT                   31
#define NCFLD_ACC_CONTROL_CS0_WR_ECC_EN_MASK                    0x40000000
#define NCFLD_ACC_CONTROL_CS0_WR_ECC_EN_SHIFT                   30
#define NCFLD_ACC_CONTROL_CS0_FAST_PGM_RDIN_MASK                0x10000000
#define NCFLD_ACC_CONTROL_CS0_FAST_PGM_RDIN_SHIFT               28
#define NCFLD_ACC_CONTROL_CS0_RD_ERASED_ECC_EN_MASK             0x08000000
#define NCFLD_ACC_CONTROL_CS0_RD_ERASED_ECC_EN_SHIFT            27
#define NCFLD_ACC_CONTROL_CS0_PARTIAL_PAGE_EN_MASK              0x04000000
#define NCFLD_ACC_CONTROL_CS0_PARTIAL_PAGE_EN_SHIFT             26
#define NCFLD_ACC_CONTROL_CS0_PAGE_HIT_EN_MASK                  0x01000000
#define NCFLD_ACC_CONTROL_CS0_PAGE_HIT_EN_SHIFT                 24
#define NCFLD_ACC_CONTROL_CS0_ECC_LEVEL_MASK                    0x001f0000
#define NCFLD_ACC_CONTROL_CS0_ECC_LEVEL_SHIFT                   16
#define NCFLD_ACC_CONTROL_CS0_SECTOR_SIZE_1K_MASK               0x00000080
#define NCFLD_ACC_CONTROL_CS0_SECTOR_SIZE_1K_SHIFT              7
#define NCFLD_ACC_CONTROL_CS0_SPARE_AREA_SIZE_MASK              0x0000007f
#define NCFLD_ACC_CONTROL_CS0_SPARE_AREA_SIZE_SHIFT             0
#define NCFLD_CORR_STAT_THRESHOLD_CS0_MASK                      0x0000003f
#define NCFLD_CORR_STAT_THRESHOLD_CS0_SHIFT                     0
#define NCFLD_CORR_STAT_THRESHOLD_CS1_MASK                      0x00000fc0
#define NCFLD_CORR_STAT_THRESHOLD_CS1_SHIFT                     6
#define NCFLD_INIT_STATUS_INIT_SUCCESS				0x20000000

/*
 * IDM NAND register offset
 */
#define IDMREG_IO_CONTROL_DIRECT				0x408
#define IDMREG_IO_STATUS					0x500
#define IDMREG_RESET_CONTROL					0x800

/*
 * Required IDM NAND IO Control register fields
 */
#define IDMFLD_NAND_IO_CONTROL_DIRECT_AXI_BE_MODE              (1UL << 28)
#define IDMFLD_NAND_IO_CONTROL_DIRECT_APB_LE_MODE              (1UL << 24)
#define IDMFLD_NAND_IO_CONTROL_DIRECT_IRQ_SHIFT                2

/*
 * Interrupts
 */
#define NCINTR_NP_READ                                          0
#define NCINTR_BLKERA                                           1
#define NCINTR_CPYBK                                            2
#define NCINTR_PGMPG                                            3
#define NCINTR_CTLRDY                                           4
#define NCINTR_RBPIN                                            5
#define NCINTR_UNC                                              6
#define NCINTR_CORR                                             7

/* 512B flash cache in the NAND controller HW */
#define FC_SHIFT            9U
#define FC_BYTES            512U
#define FC_WORDS            (FC_BYTES >> 2)
#define FC(x)               (NCREG_FLASH_CACHE_BASE + ((x) << 2))

/* 64B oob cache in the NAND controller HW */
#define MAX_CONTROLLER_OOB_BYTES        64
#define MAX_CONTROLLER_OOB_WORDS        (MAX_CONTROLLER_OOB_BYTES >> 2)

/*
 * Register access macros - NAND flash controller
 */
#define NAND_REG_RD(x)		readl(ctrl.nand_regs + (x))
#define NAND_REG_WR(x, y)	writel((y), ctrl.nand_regs + (x))
#define NAND_REG_CLR(x, y)	NAND_REG_WR((x), NAND_REG_RD(x) & ~(y))
#define NAND_REG_SET(x, y)	NAND_REG_WR((x), NAND_REG_RD(x) | (y))

/*
 * IRQ operations
 */
#define NAND_ENABLE_IRQ(bit)						     \
	writel(readl(ctrl.nand_idm_io_ctrl_direct_reg) |		     \
	       (1UL << ((bit) + IDMFLD_NAND_IO_CONTROL_DIRECT_IRQ_SHIFT)),   \
	       ctrl.nand_idm_io_ctrl_direct_reg)

#define NAND_DISABLE_IRQ(bit)						     \
	writel(readl(ctrl.nand_idm_io_ctrl_direct_reg) &		     \
	       ~(1UL << ((bit) + IDMFLD_NAND_IO_CONTROL_DIRECT_IRQ_SHIFT)),  \
	       ctrl.nand_idm_io_ctrl_direct_reg)

#define NAND_ACK_IRQ(bit) writel(1, ((u32 *)ctrl.nand_intr_regs) + (bit))

#define NAND_TEST_IRQ(bit) (readl(((u32 *)ctrl.nand_intr_regs) + (bit)) & 1)

/*
 * Data access macros for endianness
 */
#ifdef __LITTLE_ENDIAN
#define NAND_BEGIN_DATA_ACCESS()					\
	writel(readl(ctrl.nand_idm_io_ctrl_direct_reg) |		\
	       IDMFLD_NAND_IO_CONTROL_DIRECT_APB_LE_MODE,		\
	       ctrl.nand_idm_io_ctrl_direct_reg)
#define NAND_END_DATA_ACCESS()						\
	writel(readl(ctrl.nand_idm_io_ctrl_direct_reg) &		\
	       ~IDMFLD_NAND_IO_CONTROL_DIRECT_APB_LE_MODE,		\
	       ctrl.nand_idm_io_ctrl_direct_reg)
#else				/* !__LITTLE_ENDIAN */
#define NAND_BEGIN_DATA_ACCESS()
#define NAND_END_DATA_ACCESS()
#endif				/* !__LITTLE_ENDIAN */

/*
 * Misc NAND controller configuration/status macros
 */
#define NC_REG_CONFIG(cs) (NCREG_CONFIG_CS0 + ((cs) << 4))

#define WR_CONFIG(cs, field, val) do {					\
	u32 reg = NC_REG_CONFIG(cs), contents = NAND_REG_RD(reg);	\
	contents &= ~(NCFLD_CONFIG_CS0_##field##_MASK);			\
	contents |= (val) << NCFLD_CONFIG_CS0_##field##_SHIFT;		\
	NAND_REG_WR(reg, contents);					\
} while (0)

#define RD_CONFIG(cs, field)						    \
	((NAND_REG_RD(NC_REG_CONFIG(cs)) & NCFLD_CONFIG_CS0_##field##_MASK) \
	>> NCFLD_CONFIG_CS0_##field##_SHIFT)

#define NC_REG_ACC_CONTROL(cs) (NCREG_ACC_CONTROL_CS0 + ((cs) << 4))

#define WR_ACC_CONTROL(cs, field, val) do {				\
	u32 reg = NC_REG_ACC_CONTROL(cs), contents = NAND_REG_RD(reg);	\
	contents &= ~(NCFLD_ACC_CONTROL_CS0_##field##_MASK);		\
	contents |= (val) << NCFLD_ACC_CONTROL_CS0_##field##_SHIFT;	\
	NAND_REG_WR(reg, contents);					\
} while (0)

#define RD_ACC_CONTROL(cs, field)					\
	((NAND_REG_RD(NC_REG_ACC_CONTROL(cs)) &				\
	 NCFLD_ACC_CONTROL_CS0_##field##_MASK)				\
	 >> NCFLD_ACC_CONTROL_CS0_##field##_SHIFT)

#define CORR_ERROR_COUNT (NAND_REG_RD(NCREG_CORR_ERROR_COUNT))
#define UNCORR_ERROR_COUNT (NAND_REG_RD(NCREG_UNCORR_ERROR_COUNT))

#define WR_CORR_THRESH(cs, val) do {					   \
	u32 contents = NAND_REG_RD(NCREG_CORR_STAT_THRESHOLD);		   \
	u32 shift = NCFLD_CORR_STAT_THRESHOLD_CS1_SHIFT * (cs);		   \
	contents &= ~(NCFLD_CORR_STAT_THRESHOLD_CS0_MASK << shift);	   \
	contents |= ((val) & NCFLD_CORR_STAT_THRESHOLD_CS0_MASK) << shift; \
	NAND_REG_WR(NCREG_CORR_STAT_THRESHOLD, contents);		   \
} while (0)

#define NC_REG_TIMING1(cs) (NCREG_TIMING_1_CS0 + ((cs) << 4))
#define NC_REG_TIMING2(cs) (NCREG_TIMING_2_CS0 + ((cs) << 4))

#define NAND_STRAP_TYPE							     \
	((readl(ctrl.nand_strap_regs) & ctrl.data->strap_type_bitfield.mask) \
	>> ctrl.data->strap_type_bitfield.shift)
#define NAND_STRAP_PAGE							     \
	((readl(ctrl.nand_strap_regs) & ctrl.data->strap_page_bitfield.mask) \
	>> ctrl.data->strap_page_bitfield.shift)

/*
 * Internal structures
 */

struct nand_strap_type {
	uint8_t sector_1k;
	uint8_t ecc_level;
	uint16_t spare_size;
};

struct nand_strap_bitfield {
	uint32_t mask;
	uint32_t shift;
};

#define ONFI_TIMING_MODES	6
struct nand_timing {
	u32 timing1;
	u32 timing2;
};

struct nand_ctrl_data {
	uint32_t chip_select_max;
	struct nand_strap_bitfield strap_type_bitfield;
	struct nand_strap_bitfield strap_page_bitfield;
	struct nand_strap_bitfield strap_width_bitfield;
	struct nand_strap_type strap_types[16];
	uint32_t strap_page_sizes[4];
	struct nand_timing onfi_tmode[ONFI_TIMING_MODES];
};

/*
 * This flag controls if WP stays on between erase/write
 * commands to mitigate flash corruption due to power glitches.
 * Values:
 * WP_NOT_USED: WP is not used or not available
 * WP_SET_BY_DEFAULT: WP is set by default, cleared for erase/write operations
 * WP_ALWAYS_CLEARED: WP is always cleared
 */
enum iproc_nand_wp_mode {
	WP_NOT_USED = 0,
	WP_SET_BY_DEFAULT = 1,
	WP_ALWAYS_CLEARED = 2,
};

struct iproc_nand_controller {
	struct nand_hw_control controller;
	int cmd_pending;
	int auto_inited;
	/*
	 * This flag indicates that existing data cache should not be used.
	 * When PAGE_HIT is enabled and a read operation is performed from
	 * the same address, the controller is not reading from the NAND,
	 * considering that data is already available in the controller data
	 * cache. In that case the correctable or uncorrectable interrupts
	 * are not asserted, so the read would appear successful.
	 * This flag will be used to temporary disable PAGE_HIT to force the
	 * data to be read again form the NAND and have correct ECC results.
	 */
	int data_cache_invalid;

	/*
	 * The percentage of the BCH ECC correction capability
	 * above which correctable errors will be reported.
	 */
	int corr_threshold_percent;

	void *nand_regs;
	void *nand_intr_regs;
	void *nand_idm_regs;
	void *nand_idm_io_ctrl_direct_reg;
	void *nand_strap_regs;

	int strap_type;
	int strap_page_size;

	unsigned int max_cs;

	/*
	 * This flag controls NAND interface timing.
	 * Values:
	 * -1:    use current timing register values
	 * [0-5]: change timing register values to comply with ONFI
	 *        timing mode [0-5]
	 */
	int tmode;

	enum iproc_nand_wp_mode wp_mode;

	struct nand_ctrl_data const *data;
};

enum iproc_nand_ecc_code {
	ECC_CODE_BCH,
	ECC_CODE_HAMMING,
};

struct iproc_nand_cfg {
	u64 device_size;
	unsigned int block_size;
	unsigned int page_size;
	unsigned int spare_area_size;
	unsigned int device_width;
	unsigned int col_adr_bytes;
	unsigned int blk_adr_bytes;
	unsigned int ful_adr_bytes;
	unsigned int sector_size_1k;
	unsigned int ecc_level;
	enum iproc_nand_ecc_code ecc_code;
};

struct iproc_nand_host {
	u32 buf[FC_WORDS];
	struct nand_chip chip;
	struct mtd_info *mtd;
	int cs;
	unsigned int last_cmd;
	unsigned int last_byte;
	u64 last_addr;
	struct iproc_nand_cfg hwcfg;
};

static struct iproc_nand_host nand_host;

static struct nand_ecclayout iproc_nand_oob_layout;

/*
 * Global variables
 */
static struct iproc_nand_controller ctrl;

/* Maximum BCH ECC level for 512B and 1024B sectors */
static const uint8_t iproc_max_bch_ecc_level[2] = { 17, 20 };

/* BCH ECC bytes required per 512B */
static const uint8_t iproc_bch_ecc_bytes[] = {
	0, 2, 4, 6, 7, 9, 11, 13, 14, 16, 18, 20, 21, 23, 25,
	27, 28, 30, 32, 34, 35
};

/*  SoC specific data */
static const struct nand_ctrl_data soc_nand_ctrl_data = {
	.chip_select_max = NAND_MAX_CS,
	.strap_type_bitfield = {
		.mask = NAND_STRAP_TYPE_MASK,
		.shift = NAND_STRAP_TYPE_SHIFT,
	},
	.strap_page_bitfield = {
		.mask = NAND_STRAP_PAGE_MASK,
		.shift = NAND_STRAP_PAGE_SHIFT,
	},
	.strap_width_bitfield = {
		.mask = NAND_STRAP_WIDTH_MASK,
		.shift = NAND_STRAP_WIDTH_SHIFT,
	},
	.strap_types = NAND_STRAP_TYPE_DATA,
	.strap_page_sizes = NAND_STRAP_PAGE_DATA,
	.onfi_tmode = NAND_TIMING_DATA,
};

/***********************************************************************
 * Internal support functions
 ***********************************************************************/
static inline int fls64(u64 x)
{
	u32 h = x >> 32;
	if (h)
		return fls(h) + 32;
	return fls(x);
}

static void iproc_nand_wp(struct mtd_info *mtd, int wp)
{
	if (ctrl.wp_mode == WP_SET_BY_DEFAULT) {
		static int old_wp = -1;
		if (old_wp != wp) {
			debug("%s: WP %s\n", __func__, wp ? "on" : "off");
			old_wp = wp;
		}
		if (wp) {
			NAND_REG_SET(NCREG_CS_NAND_SELECT,
				     NCFLD_CS_NAND_SELECT_WP);
		} else {
			NAND_REG_CLR(NCREG_CS_NAND_SELECT,
				     NCFLD_CS_NAND_SELECT_WP);
		}
	}
}

/* Helper functions for reading and writing OOB registers */
static inline unsigned char oob_reg_read(int offs)
{
	if (offs >= MAX_CONTROLLER_OOB_BYTES)
		return 0x77;

	/* APB read is big endian */
	return NAND_REG_RD(NCREG_SPARE_AREA_READ_OFS_0 + (offs & ~0x03))
	    >> (24 - ((offs & 0x03) << 3));
}

static int wait_for_completion_timeout(int us)
{
	BUG_ON(ctrl.cmd_pending == 0);
	while (us > 0) {
		if (NAND_TEST_IRQ(NCINTR_CTLRDY)) {
			NAND_ACK_IRQ(NCINTR_CTLRDY);
			break;
		}
		__udelay(10);
		us -= 10;
	}
	return us;
}

static void iproc_nand_send_cmd(int cmd)
{
	debug("%s: native cmd %d addr_lo 0x%lx\n", __func__, cmd,
	      (unsigned long)NAND_REG_RD(NCREG_CMD_ADDRESS));
	BUG_ON(ctrl.cmd_pending != 0);
	ctrl.cmd_pending = cmd;

	/* mb(); */

	NAND_REG_WR(NCREG_CMD_START, cmd << NCFLD_CMD_START_OPCODE_SHIFT);
}

/***********************************************************************
 * NAND MTD API: read/program/erase
 ***********************************************************************/

static void
iproc_nand_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	/* intentionally left blank */
}

static int iproc_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *this)
{
	struct nand_chip *chip = mtd->priv;
	struct iproc_nand_host *host = chip->priv;

	debug("%s: native cmd %d\n", __func__, ctrl.cmd_pending);
	if (ctrl.cmd_pending &&
	    wait_for_completion_timeout(200000) <= 0) {
		printf(DRV_NAME ": timeout waiting for command %u (%ld)\n",
		       host->last_cmd,
		       (unsigned long)NAND_REG_RD(NCREG_CMD_START) >> 24);
		printf(DRV_NAME ": irq status %08lx, intfc status %08lx\n",
		       (unsigned long)NAND_TEST_IRQ(NCINTR_CTLRDY),
		       (unsigned long)NAND_REG_RD(NCREG_INTFC_STATUS));
	}
	ctrl.cmd_pending = 0;
	return NAND_REG_RD(NCREG_INTFC_STATUS) &
	    NCFLD_INTFC_STATUS_FLASH_STATUS_MASK;
}

static void
iproc_nand_cmdfunc(struct mtd_info *mtd, unsigned command,
		   int column, int page_addr)
{
	struct nand_chip *chip = mtd->priv;
	struct iproc_nand_host *host = chip->priv;
	u64 addr = (u64) page_addr << chip->page_shift;
	int native_cmd = 0;

	if (command == NAND_CMD_READID || command == NAND_CMD_PARAM)
		addr = (u64) column;

	debug("%s: cmd 0x%x addr 0x%llx\n", __func__, command,
	      (unsigned long long)addr);
	host->last_cmd = command;
	host->last_byte = 0;
	host->last_addr = addr;

	switch (command) {
	case NAND_CMD_RESET:
		native_cmd = CMD_FLASH_RESET;
		break;
	case NAND_CMD_STATUS:
		native_cmd = CMD_STATUS_READ;
		break;
	case NAND_CMD_READID:
		native_cmd = CMD_DEVICE_ID_READ;
		break;
	case NAND_CMD_READOOB:
		native_cmd = CMD_SPARE_AREA_READ;
		break;
	case NAND_CMD_ERASE1:
		native_cmd = CMD_BLOCK_ERASE;
		iproc_nand_wp(mtd, 0);
		break;
	case NAND_CMD_PARAM:
		native_cmd = CMD_PARAMETER_READ;
		break;
	default:
		return;
	}

	NAND_REG_WR(NCREG_CMD_EXT_ADDRESS,
		    (host->cs << 16) | ((addr >> 32) & 0xffff));
	NAND_REG_WR(NCREG_CMD_ADDRESS, addr & 0xffffffff);

	iproc_nand_send_cmd(native_cmd);
	iproc_nand_waitfunc(mtd, chip);

	if (command == NAND_CMD_ERASE1)
		iproc_nand_wp(mtd, 1);
}

static void iproc_nand_select_chip(struct mtd_info *mtd, int cs)
{
	struct nand_chip *chip = mtd->priv;
	struct iproc_nand_host *host = chip->priv;

	if (cs < ctrl.max_cs) {
		debug("%s: cs %d\n", __func__, cs);
		host->cs = cs;
	} else {
		printf(DRV_NAME ": invalid cs %d ignored\n", cs);
	}
}

static uint8_t iproc_nand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct iproc_nand_host *host = chip->priv;
	uint8_t ret = 0;

	switch (host->last_cmd) {
	case NAND_CMD_READID:
		if (host->last_byte < 4)
			ret = NAND_REG_RD(NCREG_FLASH_DEVICE_ID) >>
			    (24 - (host->last_byte << 3));
		else if (host->last_byte < 8)
			ret = NAND_REG_RD(NCREG_FLASH_DEVICE_ID_EXT) >>
			    (56 - (host->last_byte << 3));
		break;

	case NAND_CMD_READOOB:
		ret = oob_reg_read(host->last_byte);
		break;

	case NAND_CMD_STATUS:
		ret = NAND_REG_RD(NCREG_INTFC_STATUS) &
		    NCFLD_INTFC_STATUS_FLASH_STATUS_MASK;
		if (ctrl.wp_mode == WP_SET_BY_DEFAULT) {
			/* Hide WP status from MTD */
			ret |= NAND_STATUS_WP;
		}
		break;

	case NAND_CMD_PARAM:
		if (host->last_byte < FC_BYTES)
			ret = NAND_REG_RD(FC(host->last_byte >> 2)) >>
			    (24 - ((host->last_byte & 0x03) << 3));
		break;
	}

	debug("%s: byte = 0x%02x\n", __func__, ret);
	host->last_byte++;

	return ret;
}

static void iproc_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	int i;

	for (i = 0; i < len; i++, buf++)
		*buf = iproc_nand_read_byte(mtd);
}

static void iproc_nand_cache_read(u32 **buf, u8 **oob, int oob_bytes)
{
	int i;
	u32 w = 0;

	debug("%s buf %p oob %p oob_bytes %d\n", __func__,
	      *buf, *oob, oob_bytes);

	if (likely(*buf)) {
		NAND_BEGIN_DATA_ACCESS();
		for (i = 0; i < FC_WORDS; i++, (*buf)++)
			**buf = NAND_REG_RD(FC(i));
		NAND_END_DATA_ACCESS();
	}

	if (*oob && oob_bytes > 0) {
		for (i = 0; i < oob_bytes; i++, (*oob)++) {
			if ((i & 0x3) == 0)
				w = NAND_REG_RD(NCREG_SPARE_AREA_READ_OFS_0 +
						i);
			/* APB read is big endian */
			**oob = w >> (24 - ((i & 0x03) << 3));
		}
	}
}

/*
 * This function counts the 0 bits in a sector data and oob bytes.
 * If the count result is less than a threshold it considers that
 * the sector is an erased sector and it sets all its data and oob
 * bits to 1.
 * The threshold is set to half of the ECC strength to allow for
 * a sector that also has some bits stuck on 1 to be correctable.
 */
static int erased_sector(struct mtd_info *mtd, u8 *buf, u8 *oob,
			 unsigned int *bitflips)
{
	struct nand_chip *chip = mtd->priv;
	struct iproc_nand_host *host = chip->priv;
	int data_bytes = 512 << host->hwcfg.sector_size_1k;
	int oob_bytes = host->hwcfg.spare_area_size <<
				host->hwcfg.sector_size_1k;
	/* Set the bitflip threshold to half of the BCH ECC strength */
	int threshold = (host->hwcfg.ecc_level <<
				host->hwcfg.sector_size_1k) / 2;
	int counter = 0;
	int i;

	debug("%s buf %p oob %p\n", __func__, buf, oob);

	if (host->hwcfg.ecc_code != ECC_CODE_BCH)
		return 0;

	/* Count bitflips in OOB first */
	for (i = 0; i < oob_bytes; i++) {
		counter += hweight8(~oob[i]);
		if (counter > threshold)
			return 0;
	}

	/* Count bitflips in data */
	for (i = 0; i < data_bytes; i++) {
		counter += hweight8(~buf[i]);
		if (counter > threshold)
			return 0;
	}

	/* Clear data and oob */
	memset(buf, 0xFF, data_bytes);
	memset(oob, 0xFF, oob_bytes);

	*bitflips = counter;
	return 1;
}

static int
iproc_nand_read(struct mtd_info *mtd,
		struct nand_chip *chip, u64 addr, unsigned int trans,
		u32 *buf, u8 *oob)
{
	struct iproc_nand_host *host = chip->priv;
	u64 start_addr = addr;
	int i;
	int oob_bytes;
	unsigned int max_bitflips, bitflips;
	unsigned int corr_error_count, uncorr_error_count;
	u32 *sector_buf = buf;
	u8 *sector_oob = oob;

	debug("%s %llx -> %p (trans %x)\n", __func__,
	      (unsigned long long)addr, buf, trans);

	BUG_ON(!oob);

	NAND_ACK_IRQ(NCINTR_UNC);
	NAND_ACK_IRQ(NCINTR_CORR);
	max_bitflips = 0;
	corr_error_count = 0;
	uncorr_error_count = 0;

	NAND_REG_WR(NCREG_CMD_EXT_ADDRESS,
		    (host->cs << 16) | ((addr >> 32) & 0xffff));

	for (i = 0; i < trans; i++, addr += FC_BYTES) {
		if (!host->hwcfg.sector_size_1k || ((i & 0x1) == 0)) {
			sector_buf = buf;
			sector_oob = oob;
		}

		NAND_REG_WR(NCREG_CMD_ADDRESS, addr & 0xffffffff);

		if (ctrl.data_cache_invalid) {
			if ((i == 0) && RD_ACC_CONTROL(host->cs, PAGE_HIT_EN))
				/*
				 * Temporary disable the PAGE_HIT to force
				 * data to be read from NAND.
				 */
				WR_ACC_CONTROL(host->cs, PAGE_HIT_EN, 0);
			else
				ctrl.data_cache_invalid = 0;
		}

		/* SPARE_AREA_READ does not use ECC, so just use PAGE_READ */
		iproc_nand_send_cmd(CMD_PAGE_READ);
		iproc_nand_waitfunc(mtd, chip);

		/* OOB bytes per sector */
		oob_bytes = (mtd->oobsize / trans) <<
				host->hwcfg.sector_size_1k;
		/* OOB bytes per 512B transfer */
		if (host->hwcfg.sector_size_1k && (i & 0x01))
			oob_bytes = max(0, oob_bytes -
					MAX_CONTROLLER_OOB_BYTES);
		oob_bytes = min(oob_bytes, MAX_CONTROLLER_OOB_BYTES);

		iproc_nand_cache_read(&buf, &oob, oob_bytes);

		if (ctrl.data_cache_invalid) {
			/* Re-enable PAGE_HIT if it was temporary disabled */
			WR_ACC_CONTROL(host->cs, PAGE_HIT_EN, 1);
			ctrl.data_cache_invalid = 0;
		}

		if (buf && (!host->hwcfg.sector_size_1k || (i & 0x1))) {
			/* Check uncorrectable errors */
			if (NAND_TEST_IRQ(NCINTR_UNC)) {
				if (erased_sector(mtd,
						  (u8 *)sector_buf,
						  sector_oob,
						  &bitflips)) {
					corr_error_count += bitflips;
					if (bitflips > max_bitflips)
						max_bitflips = bitflips;
				} else {
					uncorr_error_count += 1;
				}
				NAND_ACK_IRQ(NCINTR_UNC);
				ctrl.data_cache_invalid = 1;
			}
			/* Check correctable errors */
			if (NAND_TEST_IRQ(NCINTR_CORR)) {
				bitflips = CORR_ERROR_COUNT;
				corr_error_count += bitflips;
				if (bitflips > max_bitflips)
					max_bitflips = bitflips;
				NAND_ACK_IRQ(NCINTR_CORR);
				ctrl.data_cache_invalid = 1;
			}
		}
	}
	if (uncorr_error_count) {
		printf(DRV_NAME ": %d uncorrectable errors at 0x%llx\n",
		       uncorr_error_count, (unsigned long long)start_addr);
		mtd->ecc_stats.failed += uncorr_error_count;
		/* NAND layer expects zero on ECC errors */
		return 0;
	}
	if (max_bitflips) {
		debug("%s: corrected %d bit errors at 0x%llx\n",
		      __func__, max_bitflips, (unsigned long long)start_addr);
		mtd->ecc_stats.corrected += corr_error_count;
		return max_bitflips;
	}

	return 0;
}

static int iproc_nand_read_page(struct mtd_info *mtd,
				struct nand_chip *chip,
				uint8_t *buf,
				int oob_required,
				int page)
{
	struct iproc_nand_host *host = chip->priv;

	BUG_ON(!buf);

	return iproc_nand_read(mtd, chip, host->last_addr,
			       mtd->writesize >> FC_SHIFT, (u32 *)buf,
			       (u8 *)chip->oob_poi);
}

static int iproc_nand_read_page_raw(struct mtd_info *mtd,
				    struct nand_chip *chip,
				    uint8_t *buf,
				    int oob_required,
				    int page)
{
	struct iproc_nand_host *host = chip->priv;
	int ret;

	BUG_ON(!buf);

	WR_ACC_CONTROL(host->cs, RD_ECC_EN, 0);
	ret = iproc_nand_read(mtd, chip, host->last_addr,
			      mtd->writesize >> FC_SHIFT,
			      (u32 *)buf,
			      (u8 *)chip->oob_poi);
	WR_ACC_CONTROL(host->cs, RD_ECC_EN, 1);
	return ret;
}

static int
iproc_nand_read_oob(struct mtd_info *mtd,
		    struct nand_chip *chip, int page)
{
	return iproc_nand_read(mtd, chip, (u64)page << chip->page_shift,
			       mtd->writesize >> FC_SHIFT,
			       NULL, (u8 *)chip->oob_poi);
}

static int
iproc_nand_read_oob_raw(struct mtd_info *mtd,
			struct nand_chip *chip, int page)
{
	struct iproc_nand_host *host = chip->priv;
	int ret;

	WR_ACC_CONTROL(host->cs, RD_ECC_EN, 0);
	ret = iproc_nand_read(mtd, chip, (u64)page << chip->page_shift,
			      mtd->writesize >> FC_SHIFT, NULL,
			      (u8 *)chip->oob_poi);
	WR_ACC_CONTROL(host->cs, RD_ECC_EN, 1);
	return ret;
}

static void iproc_nand_cache_write(const u32 **buf, u8 **oob, int oob_bytes)
{
	int i;
	u32 w = 0;

	debug("%s buf %p oob %p oob_bytes %d\n", __func__,
	      *buf, *oob, oob_bytes);

	if (*buf) {
		NAND_BEGIN_DATA_ACCESS();
		for (i = 0; i < FC_WORDS; i++, (*buf)++)
			NAND_REG_WR(FC(i), **buf);
		NAND_END_DATA_ACCESS();
	} else {
		for (i = 0; i < FC_WORDS; i++)
			NAND_REG_WR(FC(i), 0xffffffff);
	}

	if (*oob) {
		for (i = 0; i < oob_bytes; i++, (*oob)++) {
			w <<= 8;
			w |= **oob;
			if ((i & 0x3) == 0x3)
				NAND_REG_WR(NCREG_SPARE_AREA_WRITE_OFS_0 +
					    (i & ~0x3), w);
		} /* fill the remaining OOB bytes with 0xFF */
		for (i = oob_bytes; i < MAX_CONTROLLER_OOB_BYTES; i++) {
			w <<= 8;
			w |= 0xFF;
			if ((i & 0x3) == 0x3)
				NAND_REG_WR(NCREG_SPARE_AREA_WRITE_OFS_0 +
					    (i & ~0x3), w);
		}
	} else {
		for (i = 0; i < MAX_CONTROLLER_OOB_WORDS; i++)
			NAND_REG_WR(NCREG_SPARE_AREA_WRITE_OFS_0 + (i << 2),
				    0xffffffff);
	}
}

static int
iproc_nand_write(struct mtd_info *mtd,
		 struct nand_chip *chip, u64 addr, const u32 *buf, u8 *oob)
{
	struct iproc_nand_host *host = chip->priv;
	unsigned int trans = mtd->writesize >> FC_SHIFT;
	unsigned int i;
	int status;
	int oob_bytes;
	int ret = 0;

	debug("%s %llx <- %p\n", __func__, (unsigned long long)addr, buf);

	if (unlikely((u32)buf & 0x03)) {
		printf(DRV_NAME ": unaligned buffer: %p\n", buf);
		buf = (u32 *)((u32)buf & ~0x03);
	}

	iproc_nand_wp(mtd, 0);

	NAND_REG_WR(NCREG_CMD_EXT_ADDRESS,
		    (host->cs << 16) | ((addr >> 32) & 0xffff));

	for (i = 0; i < trans; i++, addr += FC_BYTES) {
		/* full address MUST be set before populating FC */
		NAND_REG_WR(NCREG_CMD_ADDRESS, addr & 0xffffffff);

		oob_bytes = 0;
		if (oob) {
			/* OOB bytes per sector */
			oob_bytes = (mtd->oobsize / trans) <<
					host->hwcfg.sector_size_1k;
			/* OOB bytes per 512B transfer */
			if (host->hwcfg.sector_size_1k && (i & 0x01))
				oob_bytes = max(0, oob_bytes -
						MAX_CONTROLLER_OOB_BYTES);
			oob_bytes = min(oob_bytes, MAX_CONTROLLER_OOB_BYTES);
		}
		iproc_nand_cache_write(&buf, &oob, oob_bytes);

		/* we cannot use SPARE_AREA_PROGRAM when PARTIAL_PAGE_EN=0 */
		iproc_nand_send_cmd(CMD_PROGRAM_PAGE);
		status = iproc_nand_waitfunc(mtd, chip);

		if (status & NAND_STATUS_FAIL) {
			printf(DRV_NAME ": program failed at %llx\n",
			       (unsigned long long)addr);
			ret = -EIO;
			break;
		}
	}
	iproc_nand_wp(mtd, 1);
	return ret;
}

static int iproc_nand_write_page(struct mtd_info *mtd,
				 struct nand_chip *chip,
				 const uint8_t *buf,
				 int oob_required)
{
	struct iproc_nand_host *host = chip->priv;
	uint8_t const *p, *e;

	BUG_ON(!buf);
	/*
	 * Don't write the page if it contains only FFs
	 * (to avoid generating ECC) since we consider it
	 * as an empty page (data could be written later).
	 */
	for (p = buf, e = p + mtd->writesize; p < e; p++)
		if (*p != 0xFF)
			break;
	if (p == e) {
		if (!oob_required)
			return 0;
		for (p = chip->oob_poi, e = p + mtd->oobsize; p < e; p++)
			if (*p != 0xFF)
				break;
		if (p == e)
			return 0;
	}

	return iproc_nand_write(mtd, chip, host->last_addr, (u32 *)buf,
				oob_required ? (u8 *)chip->oob_poi : NULL);
}

static int iproc_nand_write_page_raw(struct mtd_info *mtd,
				     struct nand_chip *chip,
				     const uint8_t *buf,
				     int oob_required)
{
	int ret;
	struct iproc_nand_host *host = chip->priv;

	BUG_ON(!buf);

	WR_ACC_CONTROL(host->cs, WR_ECC_EN, 0);
	ret = iproc_nand_write(mtd, chip, host->last_addr, (u32 *)buf,
			       oob_required ? (u8 *)chip->oob_poi : NULL);
	WR_ACC_CONTROL(host->cs, WR_ECC_EN, 1);
	return ret;
}

static int
iproc_nand_write_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	return iproc_nand_write(mtd, chip, (u64)page << chip->page_shift, NULL,
				(u8 *)chip->oob_poi);
}

static int
iproc_nand_write_oob_raw(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	struct iproc_nand_host *host = chip->priv;
	int ret;

	WR_ACC_CONTROL(host->cs, WR_ECC_EN, 0);
	ret = iproc_nand_write(mtd, chip, (u64)page << chip->page_shift, NULL,
			       (u8 *)chip->oob_poi);
	WR_ACC_CONTROL(host->cs, WR_ECC_EN, 1);
	return ret;
}

static int iproc_nand_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	struct nand_chip *chip = mtd->priv;
	struct iproc_nand_host *host = chip->priv;
	u64 addr = (u64)ofs;
	u32 w;
	u8 bad;

	debug("%s %llx\n", __func__, (unsigned long long)ofs);

	NAND_REG_WR(NCREG_CMD_EXT_ADDRESS,
		    (host->cs << 16) | ((addr >> 32) & 0xffff));
	NAND_REG_WR(NCREG_CMD_ADDRESS, addr & 0xffffffff);
	iproc_nand_send_cmd(CMD_PAGE_READ);
	iproc_nand_waitfunc(mtd, chip);

	/*
	 * Force next read to be done from NAND,
	 * to have correct ECC correction status.
	 */
	ctrl.data_cache_invalid = 1;

	/* APB read is big endian */
	w = NAND_REG_RD(NCREG_SPARE_AREA_READ_OFS_0 +
		(chip->badblockpos & ~0x3));
	bad = w >> (24 - ((chip->badblockpos & 0x03) << 3));
	if (bad != 0xFF) {
		debug("%s %llx BAD BLOCK\n", __func__,
		      (unsigned long long)ofs);
		return 1;
	}
	return 0;
}

static int iproc_nand_mark_bad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip *chip = mtd->priv;
	u64 blk_addr = (u64)ofs;

	debug("%s %llx\n", __func__, (unsigned long long)ofs);

	/* Get NAND block address */
	blk_addr &= ~((1 << chip->phys_erase_shift) - 1);

	memset(chip->oob_poi, 0xff, mtd->oobsize);
	chip->oob_poi[chip->badblockpos] = 0;

	return iproc_nand_write(mtd, chip, blk_addr, NULL,
				(u8 *)chip->oob_poi);
}

/***********************************************************************
 * Per-CS setup (1 NAND device)
 ***********************************************************************/

static const unsigned int block_sizes[] = { 8, 16, 128, 256, 512, 1024, 2048 };
static const unsigned int page_sizes[] = { 512, 2048, 4096, 8192 };

static void
iproc_nand_set_cfg(struct iproc_nand_host *host, struct iproc_nand_cfg *cfg)
{
	int i, found;

	for (i = 0, found = 0; i < ARRAY_SIZE(block_sizes); i++)
		if ((block_sizes[i] << 10) == cfg->block_size) {
			WR_CONFIG(host->cs, BLOCK_SIZE, i);
			found = 1;
		}
	if (!found)
		printf(DRV_NAME ": invalid block size %u\n",
		       cfg->block_size);

	for (i = 0, found = 0; i < ARRAY_SIZE(page_sizes); i++)
		if (page_sizes[i] == cfg->page_size) {
			WR_CONFIG(host->cs, PAGE_SIZE, i);
			found = 1;
		}
	if (!found)
		printf(DRV_NAME ": invalid page size %u\n",
		       cfg->page_size);

	if (fls64(cfg->device_size) < 23)
		printf(DRV_NAME ": invalid device size 0x%llx\n",
		       (unsigned long long)cfg->device_size);

	if (cfg->ecc_code == ECC_CODE_BCH) {
		if ((cfg->ecc_level >
		     iproc_max_bch_ecc_level[cfg->sector_size_1k]) ||
		    (iproc_bch_ecc_bytes[cfg->ecc_level] >
		     cfg->spare_area_size))
			printf(DRV_NAME ": invalid BCH ECC configuration: %u/%u\n",
			       cfg->ecc_level << cfg->sector_size_1k,
			       cfg->sector_size_1k ? 1024 : 512);
	}

	if (cfg->ecc_code == ECC_CODE_HAMMING) {
		if (!((cfg->ecc_level == 15) &&
		      (cfg->spare_area_size == 16) &&
		      (cfg->sector_size_1k == 0)))
			printf(DRV_NAME ": invalid HAMMING ECC configuration\n");
	}

	WR_CONFIG(host->cs, DEVICE_SIZE, fls64(cfg->device_size) - 23);
	WR_CONFIG(host->cs, DEVICE_WIDTH, cfg->device_width == 16 ? 1 : 0);
	WR_CONFIG(host->cs, COL_ADR_BYTES, cfg->col_adr_bytes);
	WR_CONFIG(host->cs, BLK_ADR_BYTES, cfg->blk_adr_bytes);
	WR_CONFIG(host->cs, FUL_ADR_BYTES, cfg->ful_adr_bytes);

	WR_ACC_CONTROL(host->cs, SPARE_AREA_SIZE, cfg->spare_area_size);
	WR_ACC_CONTROL(host->cs, SECTOR_SIZE_1K, cfg->sector_size_1k);

	WR_ACC_CONTROL(host->cs, ECC_LEVEL, cfg->ecc_level);
}

static void
iproc_nand_get_cfg(struct iproc_nand_host *host, struct iproc_nand_cfg *cfg)
{
	cfg->block_size = RD_CONFIG(host->cs, BLOCK_SIZE);
	cfg->device_size = (4ULL << 20) << RD_CONFIG(host->cs, DEVICE_SIZE);
	cfg->page_size = RD_CONFIG(host->cs, PAGE_SIZE);
	cfg->device_width = RD_CONFIG(host->cs, DEVICE_WIDTH) ? 16 : 8;
	cfg->col_adr_bytes = RD_CONFIG(host->cs, COL_ADR_BYTES);
	cfg->blk_adr_bytes = RD_CONFIG(host->cs, BLK_ADR_BYTES);
	cfg->ful_adr_bytes = RD_CONFIG(host->cs, FUL_ADR_BYTES);
	cfg->spare_area_size = RD_ACC_CONTROL(host->cs, SPARE_AREA_SIZE);
	cfg->sector_size_1k = RD_ACC_CONTROL(host->cs, SECTOR_SIZE_1K);
	cfg->ecc_level = RD_ACC_CONTROL(host->cs, ECC_LEVEL);

	if (cfg->block_size < ARRAY_SIZE(block_sizes))
		cfg->block_size = block_sizes[cfg->block_size] << 10;
	else
		cfg->block_size = 128 << 10;

	if (cfg->page_size < ARRAY_SIZE(page_sizes))
		cfg->page_size = page_sizes[cfg->page_size];
	else
		cfg->page_size = 2048;

	/* Special case: using Hamming code */
	if ((cfg->ecc_level == 15) && (cfg->spare_area_size == 16) &&
	    (cfg->sector_size_1k == 0))
		cfg->ecc_code = ECC_CODE_HAMMING;
	else
		cfg->ecc_code = ECC_CODE_BCH;
}

static void iproc_nand_print_cfg(struct iproc_nand_cfg *cfg)
{
	printf("NAND %u-bit %lluMiB total, %uKiB blocks, %u%s pages\n"
	       "     %ubit/%uB %s-ECC %uB/512B OOB\n",
	       cfg->device_width,
	       (unsigned long long)cfg->device_size >> 20,
	       cfg->block_size >> 10,
	       cfg->page_size >= 1024 ? cfg->page_size >> 10 : cfg->page_size,
	       cfg->page_size >= 1024 ? "KiB" : "B",
	       cfg->ecc_code == ECC_CODE_HAMMING ?
				1 : cfg->ecc_level << cfg->sector_size_1k,
	       512 << cfg->sector_size_1k,
	       cfg->ecc_code == ECC_CODE_HAMMING ? "HAMMING" : "BCH",
	       cfg->spare_area_size);
}

static int iproc_nand_setup_dev(struct iproc_nand_host *host)
{
	struct mtd_info *mtd = host->mtd;
	struct nand_chip *chip = &host->chip;
	struct iproc_nand_cfg orig_cfg, new_cfg;
	struct nand_oobfree *free = iproc_nand_oob_layout.oobfree;

	uint8_t steps;
	uint8_t eccbytes;
	uint8_t eccstrength;
	int threshold;

	iproc_nand_get_cfg(host, &orig_cfg);
	host->hwcfg = orig_cfg;

	memset(&new_cfg, 0, sizeof(new_cfg));
	new_cfg.device_size = mtd->size;
	new_cfg.block_size = mtd->erasesize;
	new_cfg.page_size = mtd->writesize;
	new_cfg.spare_area_size = mtd->oobsize / (mtd->writesize >> FC_SHIFT);
	new_cfg.device_width = (chip->options & NAND_BUSWIDTH_16) ? 16 : 8;
	new_cfg.col_adr_bytes = 2;

	if (mtd->writesize > 512)
		if (mtd->size >= (256 << 20))
			new_cfg.blk_adr_bytes = 3;
		else
			new_cfg.blk_adr_bytes = 2;
	else if (mtd->size >= (64 << 20))
		new_cfg.blk_adr_bytes = 3;
	else
		new_cfg.blk_adr_bytes = 2;
	new_cfg.ful_adr_bytes = new_cfg.blk_adr_bytes + new_cfg.col_adr_bytes;

	debug("%s: mtd configuration\n"
	      "\tdevice_size 0x%llx\n"
	      "\tblock_size 0x%x\n"
	      "\tpage_size 0x%x\n"
	      "\tdevice_width 0x%x\n"
	      "\tcol_adr_bytes 0x%x\n"
	      "\tblk_adr_bytes 0x%x\n"
	      "\tspare_area_size 0x%x\n"
	      "\tecc_level 0x%x\n"
	      "\tsector_size_1k 0x%x\n",
	      __func__,
	      new_cfg.device_size,
	      new_cfg.block_size,
	      new_cfg.page_size,
	      new_cfg.device_width,
	      new_cfg.col_adr_bytes,
	      new_cfg.blk_adr_bytes,
	      new_cfg.spare_area_size,
	      new_cfg.ecc_level,
	      new_cfg.sector_size_1k);

	/* Check settings determined by controller auto-init */
	if (ctrl.auto_inited) {
		debug("%s: auto-init configuration\n"
		      "\tdevice_size 0x%llx\n"
		      "\tblock_size 0x%x\n"
		      "\tpage_size 0x%x\n"
		      "\tdevice_width 0x%x\n"
		      "\tcol_adr_bytes 0x%x\n"
		      "\tblk_adr_bytes 0x%x\n"
		      "\tspare_area_size 0x%x\n"
		      "\tecc_level 0x%x\n"
		      "\tsector_size_1k 0x%x\n",
		      __func__,
		      orig_cfg.device_size,
		      orig_cfg.block_size,
		      orig_cfg.page_size,
		      orig_cfg.device_width,
		      orig_cfg.col_adr_bytes,
		      orig_cfg.blk_adr_bytes,
		      orig_cfg.spare_area_size,
		      orig_cfg.ecc_level,
		      orig_cfg.sector_size_1k);
		/* Check basic device attributes first */
		if (orig_cfg.device_size != new_cfg.device_size ||
		    orig_cfg.block_size != new_cfg.block_size ||
		    orig_cfg.page_size != new_cfg.page_size ||
		    orig_cfg.device_width != new_cfg.device_width ||
		    orig_cfg.col_adr_bytes != new_cfg.col_adr_bytes ||
		    orig_cfg.blk_adr_bytes != new_cfg.blk_adr_bytes ||
		    orig_cfg.ful_adr_bytes != new_cfg.ful_adr_bytes ||
		    orig_cfg.ecc_level == 0 ||
		    ((orig_cfg.ecc_code == ECC_CODE_BCH) &&
		     (orig_cfg.ecc_level >
		      iproc_max_bch_ecc_level[orig_cfg.sector_size_1k])) ||
		    orig_cfg.spare_area_size > new_cfg.spare_area_size ||
		    ((orig_cfg.ecc_code == ECC_CODE_BCH) &&
		     (iproc_bch_ecc_bytes[orig_cfg.ecc_level] >
		      orig_cfg.spare_area_size))) {
			/* Ignore invalid auto-init settings */
			ctrl.auto_inited = 0;
			printf(DRV_NAME ": invalid auto-init settings\n");

		} else {
			/* Auto-init has initialized the flash correctly. */
			new_cfg = orig_cfg;
			printf(DRV_NAME ": following auto-init settings\n");
		}
	}

	/* Decide ECC settings ourselves if it's not initialized before */
	if (!ctrl.auto_inited) {
		printf(DRV_NAME " straps: page 0x%x type 0x%x\n",
		       ctrl.strap_page_size, ctrl.strap_type);

		/* Check if strap settings are valid */
		if (ctrl.strap_type > 0 &&
		    ctrl.data->strap_page_sizes[ctrl.strap_page_size] ==
		    new_cfg.page_size &&
		    ctrl.data->strap_types[ctrl.strap_type].spare_size <=
		    new_cfg.spare_area_size) {
			/* It's valid, follow the strap settings */
			new_cfg.spare_area_size =
			    ctrl.data->strap_types[ctrl.strap_type].spare_size;
			new_cfg.sector_size_1k =
			    ctrl.data->strap_types[ctrl.strap_type].sector_1k;
			new_cfg.ecc_level =
			    ctrl.data->strap_types[ctrl.strap_type].ecc_level;
			if (ctrl.strap_page_size == 0) {
				new_cfg.blk_adr_bytes = 2;
				new_cfg.ful_adr_bytes = 4;
			} else {
				new_cfg.blk_adr_bytes = 3;
				new_cfg.ful_adr_bytes = 5;
			}

			/* Special case: using Hamming code */
			if ((new_cfg.ecc_level == 15) &&
			    (new_cfg.spare_area_size == 16) &&
			    (new_cfg.sector_size_1k == 0))
				new_cfg.ecc_code = ECC_CODE_HAMMING;
			else
				new_cfg.ecc_code = ECC_CODE_BCH;

			printf(DRV_NAME ": following strap settings\n");

		} else {
			/*
			 * Strap settings are not valid,
			 * decide the settings on our own
			 */

			/* Trying to fit with available strap settings */
			new_cfg.spare_area_size =
			    new_cfg.spare_area_size >= 27 ? 27 : 16;
			new_cfg.sector_size_1k = 0;
			new_cfg.ecc_code = ECC_CODE_BCH;
			if (new_cfg.spare_area_size == 27) {
				new_cfg.ecc_level = 12;
				new_cfg.sector_size_1k =
				    (new_cfg.page_size >= 2048) ? 1 : 0;
			} else if (chip->badblockpos ==
				   NAND_SMALL_BADBLOCK_POS) {
				new_cfg.ecc_level = 4;
			} else {
				new_cfg.ecc_level = 8;
			}

			printf(DRV_NAME ": overriding invalid strap settings\n");
		}

		iproc_nand_set_cfg(host, &new_cfg);
		host->hwcfg = new_cfg;
	}

	iproc_nand_print_cfg(&new_cfg);

	WR_ACC_CONTROL(host->cs, RD_ECC_EN, 1);
	WR_ACC_CONTROL(host->cs, WR_ECC_EN, 1);
	WR_ACC_CONTROL(host->cs, FAST_PGM_RDIN, 0);
	WR_ACC_CONTROL(host->cs, RD_ERASED_ECC_EN, 0);
	WR_ACC_CONTROL(host->cs, PARTIAL_PAGE_EN, 0);
	WR_ACC_CONTROL(host->cs, PAGE_HIT_EN, 1);

	if (new_cfg.ecc_code == ECC_CODE_BCH) {
		/* threshold = ceil(ECC-strength * percentage) */
		threshold = ((new_cfg.ecc_level << new_cfg.sector_size_1k) *
			     ctrl.corr_threshold_percent + 99) / 100;
		WR_CORR_THRESH(host->cs, threshold);
		printf(DRV_NAME
		       ": ECC correction status threshold set to %d bit\n",
		       threshold);
	} else {
		WR_CORR_THRESH(host->cs, 1);
	}

	/* mb(); */

	/* Adjust MTD oobsize according to the configuration */
	mtd->oobsize = new_cfg.spare_area_size * (mtd->writesize >> FC_SHIFT);

	/* Adjust ECC layout for storing usb OOB data */
	free->length = 0;
	steps = mtd->writesize >> FC_SHIFT;
	if (new_cfg.ecc_code == ECC_CODE_HAMMING) {
		eccbytes = 3;
		eccstrength = 1;
	} else {
		eccbytes = iproc_bch_ecc_bytes[new_cfg.ecc_level];
		eccstrength = new_cfg.ecc_level << new_cfg.sector_size_1k;
	}

	/*
	 * These are not really used.
	 * We still prepare them for safety.
	 */
	iproc_nand_oob_layout.eccbytes = eccbytes * steps;
	chip->ecc.bytes = eccbytes;
	chip->ecc.strength = eccstrength;
	chip->ecc.size = 512 << new_cfg.sector_size_1k;

	/* Create oobfree for storing user OOB data */
	if (new_cfg.spare_area_size > eccbytes) {
		unsigned int spare_size;
		uint8_t i, cnt;

		spare_size =
		    new_cfg.spare_area_size << new_cfg.sector_size_1k;
		eccbytes <<= new_cfg.sector_size_1k;
		steps >>= new_cfg.sector_size_1k;
		if (steps > MTD_MAX_OOBFREE_ENTRIES_LARGE)
			steps = MTD_MAX_OOBFREE_ENTRIES_LARGE;
		for (i = 0, cnt = 0;
		     i < steps && cnt < MTD_MAX_OOBFREE_ENTRIES_LARGE; i++) {
			if (new_cfg.ecc_code == ECC_CODE_HAMMING) {
				/*
				 * Hamming code: ECC bytes are 6~8;
				 * First part here.
				 */
				free->offset = i * spare_size;
				free->length = 6;

			} else {
				/* BCH: ECC bytes at the bottom */
				free->offset = i * spare_size;
				free->length = spare_size - eccbytes;
			}

			/* Reserve the first two bytes of the page */
			if (i == 0) {
				if (free->length <= 2) {
					/*
					 * Don't claim this entry if
					 * less than 2 bytes
					 */
					continue;
				}
				free->offset += 2;
				free->length -= 2;
			}

			if (new_cfg.ecc_code == ECC_CODE_HAMMING) {
				/* Hamming code: the 2nd free part */
				free++;
				cnt++;
				if (cnt < MTD_MAX_OOBFREE_ENTRIES_LARGE) {
					free->offset =
					    i * spare_size + 9;
					free->length = 7;
				} else {
					/* The structure limits us. */
					break;
				}
			}

			free++;
			cnt++;
		}
		if (cnt < MTD_MAX_OOBFREE_ENTRIES_LARGE) {
			/* Terminater */
			free->length = 0;
		}

		/* Print out oob space information */
		free = iproc_nand_oob_layout.oobfree;
		if (free->length) {
			spare_size = 0;
			while (free->length) {
				spare_size += free->length;
				free++;
			}
			printf(DRV_NAME ": user oob per page: %u bytes (%u steps)\n",
			       spare_size, (int)steps);
		}
	}

	if (iproc_nand_oob_layout.oobfree[0].length == 0)
		printf(DRV_NAME ": no oob space available\n");

	return 0;
}

static void iproc_nand_timing_setup(struct iproc_nand_host *host)
{
	struct nand_chip *chip = &host->chip;
	int onfi_tmode; /* bit mask of supported onfi timing modes */

	/*
	 * ctrl.tmode has the configured tmode upper limit [0-5]
	 * or is -1 to indicate power-on default timing
	 */
	if (ctrl.tmode < 0 || ctrl.tmode >= ONFI_TIMING_MODES)
		return;

	if (chip->onfi_version) {
		onfi_tmode = le16_to_cpu(get_unaligned(
				(u16 *)&chip->onfi_params.async_timing_mode));
		if ((onfi_tmode == 0) || (onfi_tmode & ~0x3F)) {
			printf(DRV_NAME
			       ": invalid ONFI timing mode ignored 0x%x\n",
			       onfi_tmode);
		} else {
			/*
			 * select the maximum supported ONFI timing mode
			 * that is lower than the configured limit
			 */
			while (ctrl.tmode > 0) {
				if (onfi_tmode & (1 << ctrl.tmode))
					break;
				ctrl.tmode--;
			}
		}
	}

	NAND_REG_WR(NC_REG_TIMING1(host->cs),
		    ctrl.data->onfi_tmode[ctrl.tmode].timing1);
	NAND_REG_WR(NC_REG_TIMING2(host->cs),
		    ctrl.data->onfi_tmode[ctrl.tmode].timing2);
	printf(DRV_NAME ": timing mode %d\n", ctrl.tmode);
}

static void iproc_nand_ctrl_setup(void)
{
#ifdef CONFIG_IPROC_NAND_AUTO_INIT
	int timeout;
#endif

	ctrl.cmd_pending = 0;
	ctrl.auto_inited = 0;

	/* Initialize SoC specific data */
	ctrl.data = &soc_nand_ctrl_data;

	/* Initialize register addresses */
	ctrl.nand_regs = (void *)REG_NAND_BASE;
	ctrl.nand_intr_regs = ctrl.nand_regs + NCREG_INTERRUPT_BASE;
	ctrl.nand_idm_regs = (void *)REG_NAND_IDM_BASE;
	ctrl.nand_idm_io_ctrl_direct_reg = ctrl.nand_idm_regs +
						IDMREG_IO_CONTROL_DIRECT;
#ifdef REG_NAND_STRAPS_BASE
	ctrl.nand_strap_regs = (void *)REG_NAND_STRAPS_BASE;
#else
	ctrl.nand_strap_regs = ctrl.nand_idm_regs + IDMREG_IO_STATUS;
#endif

	/* Get strap values */
	ctrl.strap_type = NAND_STRAP_TYPE;
	ctrl.strap_page_size = NAND_STRAP_PAGE;

#ifdef CONFIG_SYS_MAX_NAND_CHIPS
#if (CONFIG_SYS_MAX_NAND_CHIPS > NAND_MAX_CS)
#error "Invalid CONFIG_SYS_MAX_NAND_CHIPS value"
#endif
	ctrl.max_cs = CONFIG_SYS_MAX_NAND_CHIPS;
#else
	ctrl.max_cs = 1;
#endif

	/* Write protect configuration */
#ifdef CONFIG_IPROC_NAND_WP_MODE
#if ((CONFIG_IPROC_NAND_WP_MODE < 0) || \
	(CONFIG_IPROC_NAND_WP_MODE > WP_ALWAYS_CLEARED))
#error "Invalid CONFIG_IPROC_NAND_WP_MODE value"
#endif
	ctrl.wp_mode = CONFIG_IPROC_NAND_WP_MODE;
#else
	ctrl.wp_mode = WP_SET_BY_DEFAULT;
#endif

	/* Timing mode configuration */
#ifdef CONFIG_IPROC_NAND_TIMING_MODE
#if ((CONFIG_IPROC_NAND_TIMING_MODE < 0) || \
	(CONFIG_IPROC_NAND_TIMING_MODE >= ONFI_TIMING_MODES))
#error "Invalid CONFIG_IPROC_NAND_TIMING_MODE value"
#endif
	ctrl.tmode = CONFIG_IPROC_NAND_TIMING_MODE;
#else
	ctrl.tmode = -1; /* Use default timing configuration */
#endif

	/*
	 * Configure the percentage of the BCH ECC correction capability
	 * above which correctable errors will be reported.
	 */
#ifdef CONFIG_IPROC_NAND_CORR_THRESHOLD_PERCENT
#if ((CONFIG_IPROC_NAND_CORR_THRESHOLD_PERCENT < 0) || \
	(CONFIG_IPROC_NAND_CORR_THRESHOLD_PERCENT > 100))
#error "Invalid CONFIG_IPROC_NAND_CORR_THRESHOLD_PERCENT value"
#endif
	ctrl.corr_threshold_percent = CONFIG_IPROC_NAND_CORR_THRESHOLD_PERCENT;
#else
	ctrl.corr_threshold_percent = 60; /* 60% of ECC strength by default */
#endif

	debug("%s: nand_regs %p\n", __func__, ctrl.nand_regs);
	debug("%s: nand_intr_regs %p\n", __func__, ctrl.nand_intr_regs);
	debug("%s: nand_idm_regs %p\n", __func__, ctrl.nand_idm_regs);
	debug("%s: nand_idm_io_ctrl_direct_reg %p\n", __func__,
	      ctrl.nand_idm_io_ctrl_direct_reg);
	debug("%s: nand_strap_regs %p\n", __func__, ctrl.nand_strap_regs);
	debug("%s: max_cs %d\n", __func__, ctrl.max_cs);
	debug("%s: wp_mode %d\n", __func__, ctrl.wp_mode);

#ifdef CONFIG_IPROC_NAND_AUTO_INIT
	/* Reset the NAND controller */
	writel(1, ctrl.nand_idm_regs + IDMREG_RESET_CONTROL);
	__udelay(1);
	writel(0, ctrl.nand_idm_regs + IDMREG_RESET_CONTROL);
	__udelay(10);

	/* execute controller auto-init */
	NAND_REG_CLR(NCREG_CS_NAND_SELECT,
		     NCFLD_CS_NAND_SELECT_AUTO_DEVID_CONFIG);
	NAND_REG_SET(NCREG_CS_NAND_SELECT,
		     NCFLD_CS_NAND_SELECT_AUTO_DEVID_CONFIG);
	timeout = 100000;
	ctrl.auto_inited = 1;
	while (timeout > 0) {
		__udelay(10);
		if (NCFLD_INIT_STATUS_INIT_SUCCESS &
		    NAND_REG_RD(NCREG_INIT_STATUS)) {
			printf(DRV_NAME ": auto-init success\n");
			break;
		}
		timeout -= 10;
	}
	if (timeout <= 0) {
		ctrl.auto_inited = 0;
		printf(DRV_NAME ": auto-init failed\n");
	}
	debug("%s: auto-init status 0x%x\n", __func__,
	      NAND_REG_RD(NCREG_INIT_STATUS));
#else
	/* Check if auto-init was done due to strap_nand_flash */
	if (NAND_REG_RD(NCREG_CS_NAND_SELECT) &
	    NCFLD_CS_NAND_SELECT_AUTO_DEVID_CONFIG)
		ctrl.auto_inited = 1;
	/* Check if auto-init was done by previous boot stage */
	if (NAND_REG_RD(NCREG_SEMAPHORE) == 0xFF)
		ctrl.auto_inited = 1;
#endif

	/* Perform basic controller initialization */
	NAND_REG_CLR(NCREG_CS_NAND_SELECT,
		     NCFLD_CS_NAND_SELECT_AUTO_DEVID_CONFIG);
	NAND_REG_CLR(NCREG_CS_NAND_SELECT,
		     NCFLD_CS_NAND_SELECT_DIRECT_ACCESS_CS_MASK);
	NAND_REG_CLR(NCREG_CS_NAND_XOR, NCFLD_CS_NAND_XOR_CS_MASK);

	if (ctrl.wp_mode == WP_ALWAYS_CLEARED) {
		/* Permanently remove write-protection */
		NAND_REG_CLR(NCREG_CS_NAND_SELECT, NCFLD_CS_NAND_SELECT_WP);
	}

	/* Clear IRQ */
	NAND_ACK_IRQ(NCINTR_CTLRDY);

	return;
}

int iproc_nand_init(void)
{
	struct iproc_nand_host *host;
	struct mtd_info *mtd;
	struct nand_chip *chip;

	NAND_REG_WR(NCREG_SEMAPHORE, 0);

	host = &nand_host;
	memset(host, 0, sizeof(*host));

	host->mtd = &nand_info[0];

	host->cs = 0;

	mtd = host->mtd;
	chip = &host->chip;

	chip->priv = host;
	mtd->priv = chip;
	mtd->name = "brcmnand.0";
	/*
	 * Set mtd bitflip threshold to 1 as the desired threshold will
	 * be set in the controller register.
	 */
	mtd->bitflip_threshold = 1;

	chip->IO_ADDR_R = (void *)0xdeadbeef;
	chip->IO_ADDR_W = (void *)0xdeadbeef;

	chip->cmd_ctrl = iproc_nand_cmd_ctrl;
	chip->cmdfunc = iproc_nand_cmdfunc;
	chip->waitfunc = iproc_nand_waitfunc;
	chip->read_byte = iproc_nand_read_byte;
	chip->read_buf = iproc_nand_read_buf;
	if (ctrl.max_cs > 1)
		chip->select_chip = iproc_nand_select_chip;

	chip->block_bad = iproc_nand_block_bad;
	chip->block_markbad = iproc_nand_mark_bad;

	chip->ecc.mode = NAND_ECC_HW;
	chip->ecc.layout = &iproc_nand_oob_layout;
	chip->ecc.read_page = iproc_nand_read_page;
	chip->ecc.write_page = iproc_nand_write_page;
	chip->ecc.read_page_raw = iproc_nand_read_page_raw;
	chip->ecc.write_page_raw = iproc_nand_write_page_raw;

	chip->ecc.write_oob_raw = iproc_nand_write_oob_raw;
	chip->ecc.read_oob_raw = iproc_nand_read_oob_raw;

	chip->ecc.read_oob = (void *)iproc_nand_read_oob;
	chip->ecc.write_oob = iproc_nand_write_oob;

	chip->controller = &ctrl.controller;

	if (nand_scan_ident(mtd, ctrl.max_cs, NULL))
		return -ENXIO;

	chip->options |= NAND_NO_SUBPAGE_WRITE | NAND_SKIP_BBTSCAN;

	chip->bbt_options |= NAND_BBT_USE_FLASH | NAND_BBT_NO_OOB;

	iproc_nand_timing_setup(host);

	if (iproc_nand_setup_dev(host) || nand_scan_tail(mtd))
		return -ENXIO;

	nand_register(0);

	/* Set semaphore to indicate initializatin done */
	NAND_REG_WR(NCREG_SEMAPHORE, 0xFF);

	printf(DRV_NAME ": NAND controller driver is loaded\n");
	return 0;
}

void board_nand_init(void)
{
	printf(DRV_NAME " controller\n");
	iproc_nand_ctrl_setup();
	if (iproc_nand_init())
		printf(DRV_NAME ": NAND initialization failed\n");
	else
		printf("NAND:  ");  /* size will be filled in by u-boot */
}

