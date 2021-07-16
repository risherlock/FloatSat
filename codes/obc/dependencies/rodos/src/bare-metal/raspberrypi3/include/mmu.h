/**
 * @file mmu.h
 * @date 2016/10/24
 * @author Michael Zehrer
 *
 * Definitions for the MMU code.
 * (Only Short-descriptor translation table format)
 * 
 * See ARMv8-A Architecture Reference Manual (Page 3580)
 *
 * Copyright 2016 University Wuerzburg
 */
#ifndef _MMU_H_
#define _MMU_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t pxn : 1;                     // privileged execute-never bit
    uint32_t _one : 1;                    // must be one!
    uint32_t b : 1;                       // bufferable
    uint32_t c : 1;                       // cacheable
    uint32_t xn : 1;                      // execute-never bit
    uint32_t domain : 4;                  // domain
    uint32_t _implementation_defined : 1; // implementation defined
    uint32_t ap1_0 : 2;                   // access permissions AP[1:0]
    uint32_t tex : 3;                     // TEX remap
    uint32_t ap2 : 1;                     // access permissions AP[2]
    uint32_t s : 1;                       // shareable
    uint32_t ng : 1;                      // not-global
    uint32_t _zero : 1;                   // must be zero!
    uint32_t ns : 1;                      // non-secure
    uint32_t base_address : 12;           // section base address
} mmu_level1_section_bitfields;

typedef union {
    uint32_t                     value;
    mmu_level1_section_bitfields bits;
} mmu_level1_section_t;


void enable_mmu();
void disable_mmu();

void init_mmu();

#ifdef __cplusplus
} // end extern "C"
#endif

#endif /* _MMU_H_ */
