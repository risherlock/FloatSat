/*=========================================================================*/
#ifndef CACHE_H_
#define CACHE_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif


/*-------------------------------------------------------------------------*/ /**
  The cc_region_size_t type is used to identify cache region size. This
  enumeration is used as parameter type by MSS_CC_set_region() to specify
  the size of cache region.
 */

typedef enum {
    CC_128M       = 0x1,
    CC_256M_LOWER = 0x3,
    CC_256M_UPPER = 0xC,
    CC_512M       = 0xF
} cc_region_size_t;
/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_enable() function enables cache memory.

  @param
    none

  @return
    none
 */
void MSS_CC_enable(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_disable() function disables cache memory.

  @param
    none

  @return
    none
 */
void MSS_CC_disable(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_enable_prefetch() function enables cache prefetch buffer.

  @param
    none

  @return
    none
 */
void MSS_CC_enable_prefetch(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_disable_prefetch() function disables cache prefetch buffer.

  @param
    none

  @return
    none
 */
void MSS_CC_disable_prefetch(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_enable_sbus_wr() function enables debug Mode SBUS writes to cache
  memory.

  @param
    none

  @return
    none
 */
void MSS_CC_enable_sbus_wr(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_disable_sbus_wr() function disables debug Mode SBUS writes to cache
   memory.

  @param
    none

  @return
    none
 */
void MSS_CC_disable_sbus_wr(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_enable_lock() function enables cache lock.

  @param
    none

  @return
    none
 */
void MSS_CC_enable_lock(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_disable_lock() function disables cache lock.

  @param
    none

  @return
    none
 */
void MSS_CC_disable_lock(
  void);

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_flush() function flushes Cache Memory which is used to
  invalidate all tags of four sets at the same time.

  @param
    none

  @return
    none
 */
void MSS_CC_flush(
  void);


/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_set_region() function sets the cache region size.

  @param cache_size
    The parameter cache_size of type cc_region_size_t is used to specify
    the size of cache region. Possible values are 128K,  256K_LOWER, 256K_UPPER,
    512K.

  @return
    none
 */
void MSS_CC_set_region(
  cc_region_size_t cache_size);


/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_get_miss_cnt() function returns the total number of cache miss
  occurs on Cacheable Region through ICode

  @return
    It returns the total number of cache miss.
 */
uint32_t
  MSS_CC_get_miss_cnt(

  );

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_get_hits_cnt() function returns the total number of cache hits
  occurs on Cacheable Region through ICode



  @return
    It returns the total number of cache hits.
 */
uint32_t
  MSS_CC_get_hits_cnt(

  );

/*-------------------------------------------------------------------------*/ /**
  The MSS_CC_get_trans_cnt() function returns the total number of transaction
  count processed by Cache Engine (Cacheable and Non Cacheable reads on
  ICode bus).



  @return
    It returns the total number of cache transaction.
 */
uint32_t
  MSS_CC_get_trans_cnt(

  );

#ifdef __cplusplus
}
#endif

#endif /* CACHE_H_ */
