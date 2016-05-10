/*****************************************************************************
* Copyright 2010 - 2013 Broadcom Corporation.  All rights reserved.
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
* @file  ccu_nand_inline.c
*
* @brief Clock Control Unit inline functions
*
* @note
*
*******************************************************************************/
#ifndef _CCU_NAND_INLINE_H_
#define _CCU_NAND_INLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ccu_kpm_inline.h"


#define CCU_NAND_PLL_SELECT_REF_CRYSTAL_CLK          0x0
#define CCU_NAND_PLL_SELECT_VAR_312M_CLK             0x1
#define CCU_NAND_PLL_SELECT_VAR_208M_CLK             0x2


#define CCU_NAND_POLICY_SET_POLICY_0                 (0x1 << ccu_policy_0)
#define CCU_NAND_POLICY_SET_POLICY_1                 (0x1 << ccu_policy_1)
#define CCU_NAND_POLICY_SET_POLICY_2                 (0x1 << ccu_policy_2)
#define CCU_NAND_POLICY_SET_POLICY_3                 (0x1 << ccu_policy_3)
#define CCU_NAND_POLICY_SET_POLICY_ALL               (CCU_NAND_POLICY_SET_POLICY_0 | \
                                                           CCU_NAND_POLICY_SET_POLICY_1 | \
                                                           CCU_NAND_POLICY_SET_POLICY_2 | \
                                                           CCU_NAND_POLICY_SET_POLICY_3 )


/******************************************************************************
* 
*  Function Name:
*  void ccu_nand_init(uint32_t policy_set,
*                          uint32_t pll_select,
*                          uint32_t divider)
* 
*  Description: This function configures clock settings for the NAND
*               host controller.
* 
*  Parameters:  set of policies to be enabled
*               clock source
*               clock divider
* 
*  Return:      void
* 
******************************************************************************/
static inline void ccu_nand_init(uint32_t policy_set,
                                      uint32_t pll_select,
                                      uint32_t divider)
{
   uint32_t access;

   /* Enable write access */
   access = ccu_unlock_kpm_clk_mgr();

   /* Configure the policies */
   ccu_cfg_kpm_policy_mask(0, ccu_kpm_policy_mask_nand, policy_set & CCU_NAND_POLICY_SET_POLICY_0);
   ccu_cfg_kpm_policy_mask(1, ccu_kpm_policy_mask_nand, policy_set & CCU_NAND_POLICY_SET_POLICY_1);
   ccu_cfg_kpm_policy_mask(2, ccu_kpm_policy_mask_nand, policy_set & CCU_NAND_POLICY_SET_POLICY_2);
   ccu_cfg_kpm_policy_mask(3, ccu_kpm_policy_mask_nand, policy_set & CCU_NAND_POLICY_SET_POLICY_3);

   /* Set clock source and divider */
   ccu_set_kpm_nand_div(ccu_kpm_nand_div_pll_sel, pll_select);      
   ccu_set_kpm_nand_div(ccu_kpm_nand_div, divider);      

   /* Enable clocks */
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_ahb_hw_sw_gating_sel, 1);
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_hw_sw_gating_sel, 1);
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_ahb_clk_en, 1);
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_clk_en, 1);

   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_voltage_level, 0);

   /* Trigger */
   ccu_set_kpm_div_trig(ccu_kpm_div_trig_nand_trig, 1);

   /* Wait for clock source switch */
   while (ccu_get_kpm_div_trig(ccu_kpm_div_trig_nand_trig) != 0);

   /* Wait for clocks to be turned on */
   while ((ccu_get_kpm_nand_clkgate(ccu_kpm_nand_clkgate_ahb_stprsts) == 0) ||
          (ccu_get_kpm_nand_clkgate(ccu_kpm_nand_clkgate_stprsts) == 0));

   /* Restore write access status*/
   ccu_restore_kpm_clk_mgr(access);
}

/******************************************************************************
* 
*  Function Name:
*  void ccu_inline_nand_deinit(void)
* 
*  Description: This function stops the clock for the NAND host controller.
* 
*  Parameters:  none
* 
*  Return:      void
* 
******************************************************************************/
static inline void ccu_nand_deinit(void)
{
   uint32_t access;

   /* Enable write access */
   access = ccu_unlock_kpm_clk_mgr();

   /* Set software controlled clock gating */
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_ahb_hw_sw_gating_sel, 1);
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_hw_sw_gating_sel, 1);

   /* Disable clocks */
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_ahb_clk_en, 0);
   ccu_set_kpm_nand_clkgate(ccu_kpm_nand_clkgate_clk_en, 0);

   /* Wait for clocks to stop */
   while ((ccu_get_kpm_nand_clkgate(ccu_kpm_nand_clkgate_ahb_stprsts) != 0) ||
          (ccu_get_kpm_nand_clkgate(ccu_kpm_nand_clkgate_stprsts) != 0));

   /* disable all policies */
   ccu_clr_kpm_policy_mask(0, ccu_kpm_policy_mask_nand);
   ccu_clr_kpm_policy_mask(1, ccu_kpm_policy_mask_nand);
   ccu_clr_kpm_policy_mask(2, ccu_kpm_policy_mask_nand);
   ccu_clr_kpm_policy_mask(3, ccu_kpm_policy_mask_nand);

   /* Restore write access status*/
   ccu_restore_kpm_clk_mgr(access);
}

#ifdef __cplusplus
}
#endif

#endif /*_CCU_NAND_INLINE_H_*/
