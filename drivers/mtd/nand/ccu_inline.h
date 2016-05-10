/*****************************************************************************
* Copyright 2008 - 2013 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/**
*
* @file  chal_ccu_inline.c
*
* @brief Clock Control Unit inline functions
*
* @note
*
*******************************************************************************/
#ifndef _CCU_INLINE_H_
#define _CCU_INLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "brcm_rdb_kpm_clk_mgr_reg.h"

#include "ccu_util_inline.h"
#include "chal_reg.h"

#define MM_IO_BASE_KPM_CLK              KONA_MST_CLK_BASE_ADDR  /* Kona Peripheral Master Clock Manager */

#define  CCU_WR_ACCESS_MODE_MASK        0x80000000
#define  CCU_WR_ACCESS_PASSWORD         0x00A5A500
#define  CCU_WR_ACCESS_ENABLE           0x00000001

#define  CCU_POLICY_CONFIG_EN_MASK      0x00000001

/* Maximum AXI frequency */
#define CCU_MAX_A9_AXI_FREQ		       300000000

/* FPGA A9 core frequency */
#define  CCU_FPGA_A9_FREQ               5000000


/* I-prefix fix up */
#define I_FIX(label)	label

/* Name fix-up */
#define NFIX(label)		I_FIX(label)



/* Active/Target policy types */
typedef enum
{
   ccu_tgt_policy = 0,   /* target policy  */
   ccu_act_policy        /* active policy  */
   
} ccu_policy_freq_type_e;

/* Generic policy numbers */
typedef enum
{
   ccu_policy_0 = 0,
   ccu_policy_1,
   ccu_policy_2,
   ccu_policy_3   
} ccu_policy_num_e;

/* Generic frequency IDs */
typedef enum
{
   ccu_freq_id_0 = 0,
   ccu_freq_id_1,
   ccu_freq_id_2,
   ccu_freq_id_3,
   ccu_freq_id_4,
   ccu_freq_id_5,
   ccu_freq_id_6,
   ccu_freq_id_7
} ccu_freq_id_e;


/* POLICY FREQ */
typedef enum
{
   /* AXI/AHB/APB4 frequencies */
   ccu_kpm_policy_freq_xtal = 0,
   ccu_kpm_policy_freq_52_52_52,
   ccu_kpm_policy_freq_104_52_26,
   ccu_kpm_policy_freq_156_156_26,
   ccu_kpm_policy_freq_156_78_39,
   ccu_kpm_policy_freq_208_104_52,
   ccu_kpm_policy_freq_312_104_52,
   ccu_kpm_policy_freq_312_156_78,
   ccu_kpm_policy_freq_not_supported
} ccu_kpm_policy_freq_e;

/* POLICY MASK */
typedef enum
{
   ccu_kpm_policy_mask_sys_switch = 0,
   ccu_kpm_policy_mask_dma,
   ccu_kpm_policy_mask_master_switch,
   ccu_kpm_policy_mask_sdio3,
   ccu_kpm_policy_mask_sdio4,
   ccu_kpm_policy_mask_sdio2,
   ccu_kpm_policy_mask_sdio1,
   ccu_kpm_policy_mask_nand,
   ccu_kpm_policy_mask_reserved8,
   ccu_kpm_policy_mask_usb_ic,
   ccu_kpm_policy_mask_reserved10,
   ccu_kpm_policy_mask_usb_otg,
   ccu_kpm_policy_mask_armcore,
   ccu_kpm_policy_mask_apb8,
   ccu_kpm_policy_mask_reserved14,
   ccu_kpm_policy_mask_reserved15,
   ccu_kpm_policy_mask_hsic2,
   ccu_kpm_policy_mask_usbh2,
   ccu_kpm_policy_mask_spi_slave,
   ccu_kpm_policy_mask_not_supported
} ccu_kpm_policy_mask_e;

/* Function Prototypes */
static inline uint32_t  ccu_unlock_clk_manager( uint32_t addr );
static inline void      ccu_lock_clk_manager( uint32_t addr );
static inline void      ccu_set_lvm_enable( uint32_t addr );
static inline void      ccu_wait_lvm_ready( uint32_t addr );
static inline uint32_t  ccu_is_lvm_ready( uint32_t addr, int timeout );

static inline void                        ccu_set_kpm_policy_freq( ccu_policy_num_e policy_num, ccu_kpm_policy_freq_e freq );
static inline ccu_kpm_policy_freq_e  ccu_get_kpm_policy_freq( uint32_t policy_num );
static inline void                        ccu_cfg_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type, uint32_t val );
static inline void                        ccu_set_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type );
static inline void                        ccu_clr_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type );
static inline uint32_t                    ccu_get_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type );
static inline ccu_policy_num_e       ccu_get_kpm_active_policy( void );

#define ccu_unlock_kpm_clk_mgr()           ccu_unlock_clk_manager( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_WR_ACCESS_OFFSET )
#define ccu_lock_kpm_clk_mgr()             ccu_lock_clk_manager( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_WR_ACCESS_OFFSET )
#define ccu_restore_kpm_clk_mgr(old_en)    ccu_restore_clk_manager( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_WR_ACCESS_OFFSET, old_en )
#define ccu_set_kpm_lvm_enable()           ccu_set_lvm_enable( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_LVM_EN_OFFSET )
#define ccu_wait_kpm_lvm_ready()           ccu_wait_lvm_ready( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_LVM_EN_OFFSET )
#define ccu_is_kpm_lvm_ready(timeout)      ccu_is_lvm_ready( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_LVM_EN_OFFSET, timeout )

/* Functions */
static inline uint32_t ccu_unlock_clk_manager( uint32_t addr )
{
   uint32_t old_en;
   uint32_t access;
   access = CHAL_REG_READ32( addr );
   old_en = access & CCU_WR_ACCESS_ENABLE;    /* save the existing ACCESS ENABLE bit */
   access &= CCU_WR_ACCESS_MODE_MASK;         /* retaining the access mode bit       */
   access |= CCU_WR_ACCESS_PASSWORD | CCU_WR_ACCESS_ENABLE;
   CHAL_REG_WRITE32( addr, access );
   return old_en;
}

static inline void ccu_lock_clk_manager( uint32_t addr )
{
   uint32_t access;
   
   access = CHAL_REG_READ32( addr );
   access &= CCU_WR_ACCESS_MODE_MASK;   /* retaining the access mode bit */
   access |= CCU_WR_ACCESS_PASSWORD;
   CHAL_REG_WRITE32( addr, access );
}

static inline void ccu_restore_clk_manager( uint32_t addr, uint32_t old_en )
{
   uint32_t access;
   
   access = CHAL_REG_READ32( addr );
   access &= CCU_WR_ACCESS_MODE_MASK;         /* retaining the access mode bit       */
   access |= CCU_WR_ACCESS_PASSWORD;
   access |= (old_en & CCU_WR_ACCESS_ENABLE); /* restore the given ACCESS ENABLE bit */
   CHAL_REG_WRITE32( addr, access );
}

static inline void ccu_set_lvm_enable( uint32_t addr )
{
   ccu_set_reg_field( addr, CCU_POLICY_CONFIG_EN_MASK, 0, CCU_POLICY_CONFIG_EN_MASK);
}

static inline void ccu_wait_lvm_ready( uint32_t addr )
{
   /* wait until all existing policy configurations are complete */
   while ( (CHAL_REG_READ32(addr) & CCU_POLICY_CONFIG_EN_MASK ) != 0 );
}

static inline uint32_t ccu_is_lvm_ready( uint32_t addr, int timeout )
{
   return( ccu_wait_for_mask_clear( addr, CCU_POLICY_CONFIG_EN_MASK, timeout ) );
}

static inline void ccu_set_kpm_policy_freq( ccu_policy_num_e policy_num, ccu_kpm_policy_freq_e freq )
{
   uint32_t access;
   
   /* enable access */
   access = ccu_unlock_kpm_clk_mgr();
   
   /* enable sw update */
   ccu_set_kpm_lvm_enable();
   
   /* wait until all existing policy configurations are complete */
   ccu_wait_kpm_lvm_ready();
   
   /* set frequency policy */
   ccu_set_reg_field( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_FREQ_OFFSET,             /* addr  */
                           KPM_CLK_MGR_REG_POLICY_FREQ_POLICY0_FREQ_MASK << (8 * policy_num),   /* mask  */
                           8 * policy_num,                                                      /* shift */
                           (uint32_t)freq ) ;                                                   /* value */

   /* trigger go */
   ccu_set_reg_field( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_CTL_OFFSET,
                           KPM_CLK_MGR_REG_POLICY_CTL_GO_ATL_MASK | KPM_CLK_MGR_REG_POLICY_CTL_GO_MASK,
                           0,
                           KPM_CLK_MGR_REG_POLICY_CTL_GO_ATL_MASK | KPM_CLK_MGR_REG_POLICY_CTL_GO_MASK );
   
   /* wait until complete */
   while( ccu_get_bit( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_CTL_OFFSET, KPM_CLK_MGR_REG_POLICY_CTL_GO_SHIFT ) != 0 );
   
   /* restore access */
   ccu_restore_kpm_clk_mgr(access);
}

static inline ccu_kpm_policy_freq_e ccu_get_kpm_policy_freq( uint32_t policy_num )
{
   uint32_t access;
   uint32_t freq_id;
   
   /* enable access */
   access = ccu_unlock_kpm_clk_mgr();
   
   /* set frequency policy */
   freq_id = ccu_get_reg_field( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_FREQ_OFFSET,            /* addr  */
                                     KPM_CLK_MGR_REG_POLICY_FREQ_POLICY0_FREQ_MASK << (8 * policy_num),  /* mask  */
                                     8 * policy_num ) ;                                                  /* shift */
   
   /* restore access */
   ccu_restore_kpm_clk_mgr(access);
   
   return (ccu_kpm_policy_freq_e)freq_id;
}

static inline void ccu_cfg_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type, uint32_t val )
{
   uint32_t addr;
   uint32_t access;
   
   /* enable access */
   access = ccu_unlock_kpm_clk_mgr();
   
   /* enable sw update */
   ccu_set_kpm_lvm_enable();
   
   /* wait until all existing policy configurations are complete */
   ccu_wait_kpm_lvm_ready();
   
   addr  = MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY0_MASK_OFFSET;
   addr += (sizeof(uint32_t) * policy_num);

   ccu_set_bit( addr, policy_type, val );
   
   /* trigger go */
   ccu_set_reg_field( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_CTL_OFFSET,
                           KPM_CLK_MGR_REG_POLICY_CTL_GO_ATL_MASK | KPM_CLK_MGR_REG_POLICY_CTL_GO_MASK,
                           0,
                           KPM_CLK_MGR_REG_POLICY_CTL_GO_ATL_MASK | KPM_CLK_MGR_REG_POLICY_CTL_GO_MASK );
   
   /* wait until complete */
   while( ccu_get_bit( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_CTL_OFFSET, KPM_CLK_MGR_REG_POLICY_CTL_GO_SHIFT ) != 0 );
   
   /* restore access */
   ccu_restore_kpm_clk_mgr(access);
}

static inline void ccu_set_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type )
{
   ccu_cfg_kpm_policy_mask( policy_num, policy_type, 1 );
}

static inline void ccu_clr_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type )
{
   ccu_cfg_kpm_policy_mask( policy_num, policy_type, 0 );
}

static inline uint32_t ccu_get_kpm_policy_mask( ccu_policy_num_e policy_num, ccu_kpm_policy_mask_e policy_type )
{
   uint32_t addr;
   uint32_t bit;
   
   addr  = MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY0_MASK_OFFSET;
   addr += (sizeof(uint32_t) * policy_num);
   bit   = ccu_get_bit( addr, policy_type );
   
   return bit;
}

static inline ccu_policy_num_e ccu_get_kpm_active_policy( void )
{
   return (ccu_policy_num_e)ccu_get_reg_field( MM_IO_BASE_KPM_CLK + KPM_CLK_MGR_REG_POLICY_DBG_OFFSET,  /* addr */
                                                         KPM_CLK_MGR_REG_POLICY_DBG_ACT_POLICY_MASK,              /* mask */
                                                         KPM_CLK_MGR_REG_POLICY_DBG_ACT_POLICY_SHIFT );           /* shift */
}

#ifdef __cplusplus
}
#endif

#endif /* _CCU_INLINE_H_*/

