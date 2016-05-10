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
* @file  ccu_util_inline.c
*
* @brief Clock Control Unit inline functions
*
* @note
*
*******************************************************************************/
#ifndef _CCU_UTIL_INLINE_H_
#define _CCU_UTIL_INLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "reg.h"
#define CCU_REG_READ32(addr) reg32_read((volatile uint32_t *)(addr))
#define CCU_REG_WRITE32(addr, val) reg32_write((volatile uint32_t *)(addr), val)
#define CCU_REG_CLRBIT32(addr, bits)  reg32_clear_bits((volatile uint32_t *)(addr), bits)
#define CCU_REG_SETBIT32(addr, bits)  reg32_set_bits((volatile uint32_t *)(addr), bits)

#define CCU_INLINE_RC_OK      0
#define CCU_INLINE_RC_FAIL    1


/* Macros to set/get multiple register bits at one time */
#define ccu_set_reg_field(addr, mask, shift, val)      \
            do                                              \
            {                                               \
               uint32_t tmp;                                \
               tmp  = CCU_REG_READ32(addr);                \
               tmp &= ~(mask);                              \
               tmp |= (((val) << (shift)) & (mask));        \
               CCU_REG_WRITE32(addr, tmp);                 \
                                                            \
            } while(0)

#define ccu_get_reg_field(addr, mask, shift)           \
            ((CCU_REG_READ32(addr) & (mask)) >> (shift))


/* Function Prototypes */
static inline void      ccu_set_bit( uint32_t addr, uint32_t bit_num, uint32_t val );
static inline uint32_t  ccu_get_bit( uint32_t addr, uint32_t bit_num );
static inline uint32_t  ccu_wait_for_mask_clear(uint32_t reg, uint32_t mask, uint32_t msec);
static inline uint32_t  ccu_inline_wait_for_mask_set(uint32_t reg, uint32_t mask, uint32_t msec);

/* Functions */
static inline void ccu_set_bit( uint32_t addr, uint32_t bit_num, uint32_t val )
{
   if( val )
   {
      CCU_REG_SETBIT32( addr, 1 << bit_num );
   }
   else
   {
      CCU_REG_CLRBIT32( addr, 1 << bit_num );
   }
}

static inline uint32_t ccu_get_bit( uint32_t addr, uint32_t bit_num )
{
   return (CCU_REG_READ32( addr ) >> bit_num) & 0x1;
}

static inline uint32_t ccu_wait_for_mask_clear(uint32_t reg, uint32_t mask, uint32_t msec)
{
    while (msec)
    {
       udelay(1000);
       msec--;
       if ((CCU_REG_READ32(reg) & mask) == 0)
       {
          return CCU_INLINE_RC_OK;
       }
    }

    if ((CCU_REG_READ32(reg) & mask))
    {
        return CCU_INLINE_RC_FAIL;
    }

    return CCU_INLINE_RC_OK;
}

static inline uint32_t ccu_inline_wait_for_mask_set(uint32_t reg, uint32_t mask, uint32_t msec)
{
    while (msec)
    {
       udelay(1000);
       msec--;
       if ((CCU_REG_READ32(reg) & mask) == mask)
       {
          return CCU_INLINE_RC_OK;
       }
    }

    if ((CCU_REG_READ32(reg) & mask) != mask)
    {
        return CCU_INLINE_RC_FAIL;
    }

    return CCU_INLINE_RC_OK;
}

#ifdef __cplusplus
}
#endif

#endif /* _CCU_UTIL_INLINE_H_*/

