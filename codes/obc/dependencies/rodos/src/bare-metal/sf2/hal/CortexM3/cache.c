/*******************************************************************************
 * (c) Copyright 2008-2010 Actel Corporation.  All rights reserved.
 * 
 * G4Main MSS Cache Controller bare metal driver implementation.
 *
 * SVN $Revision: 4229 $
 * SVN $Date: 2012-03-28 17:53:50 +0530 (Wed, 28 Mar 2012) $
 */
#include "cache.h"

#include "m2sxxx.h"
#ifdef __cplusplus
extern "C" {
#endif


/*-------------------------------------------------------------------------*/ /**
 * Mask Definitions
 */
#define SYSREG_CACHE_ENABLE_MASK (uint32_t)0x0000001u
#define SYSREG_CACHE_SBUS_WR_MODE_MASK (uint32_t)0x0000002u
#define SYSREG_CACHE_LOCK_MASK (uint32_t)0x0000004u
#define SYSREG_CACHE_FLUSH_MASK (uint32_t)0x0000001u

/*-------------------------------------------------------------------------*/ /**
 * See "cache.h" for details of how to use this function.
 */
void MSS_CC_enable(
  void) {
    SYSREG->CC_CR |= SYSREG_CACHE_ENABLE_MASK;
}
/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
void MSS_CC_disable(
  void) {
    SYSREG->CC_CR = 0;
}


/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
void MSS_CC_enable_lock(
  void) {
    SYSREG->CC_CR |= SYSREG_CACHE_LOCK_MASK;
}

/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
void MSS_CC_disable_lock(
  void) {
    SYSREG->CC_CR &= ~SYSREG_CACHE_LOCK_MASK;
}

/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
void MSS_CC_flush(
  void) {
    SYSREG->FLUSH_CR = SYSREG_CACHE_FLUSH_MASK;
    //*(volatile uint32_t *)0x400381A8 |= 0x1;
}


/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
void MSS_CC_set_region(
  cc_region_size_t cache_size) {
    SYSREG->CC_REGION_CR = (0xF & (uint8_t)cache_size);
}


/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
uint32_t
  MSS_CC_get_miss_cnt(

  ) {

    return (SYSREG->CC_IC_MISS_CNT_SR);
}

/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
uint32_t
  MSS_CC_get_hits_cnt(

  ) {

    return (SYSREG->CC_IC_HIT_CNT_SR);
}

/*-------------------------------------------------------------------------*/ /**
 * See "mss_cache.h" for details of how to use this function.
 */
uint32_t
  MSS_CC_get_trans_cnt(

  ) {

    return (SYSREG->CC_IC_TRANS_CNT_SR);
}

#ifdef __cplusplus
}
#endif
