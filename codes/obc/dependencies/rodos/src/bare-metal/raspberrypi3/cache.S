/**
 * @file cache.S
 * @date 2016/09/30
 * @author Michael Zehrer, Sandro Pinto
 *
 * Copyright 2015 AIT & ESRG
 * Copyright 2016 University Wuerzburg
 */
#include "include/asm_defines.h"

.section ".text"

/* loads a 32-bit value into a register without a data access */
.macro mov32, reg, val
	movw	\reg, #:lower16:\val
	movt	\reg, #:upper16:\val
.endm

/**
 * ONLY SOME INFORMATION
 * According to the ARM ABI
 *
 * r0-r1 are used for passing the first two arguments to functions, and returning the results of functions. If a function does not use them for a return value, they can take any value after a function.
 * r2-r3 are used for passing the second two arguments to functions. There values after a function is called can be anything.
 * r4-r12 are used for working values, and their value after a function is called must be the same as before. 
 */

.global enable_dcache
.func enable_dcache
enable_dcache:
    dmb
	// read the system control register (SCTLR)
    mrc p15, 0, r0, c1, c0, 0
    // set the c bit
    orr r0, r0, #SCTLR_C
    // write changes to system control register (SCTLR)
    mcr p15, 0, r0, c1, c0, 0
    // complete all pending memory accesses
    dsb
    bx lr
.endfunc

.global disable_dcache
.func disable_dcache
disable_dcache:
    dmb
	// read the system control register (SCTLR)
    mrc p15, 0, r0, c1, c0, 0
    // clear the c bit
    bic r0, r0, #SCTLR_C
    // write changes to system control register (SCTLR)
    mcr p15, 0, r0, c1, c0, 0
    // complete all pending memory accesses
    dsb
    bx lr
.endfunc

.global enable_icache
.func enable_icache
enable_icache:
    dmb
	// read the system control register (SCTLR)
    mrc p15, 0, r0, c1, c0, 0
    // set the i bit
    orr r0, r0, #SCTLR_I
    // write changes to system control register (SCTLR)
    mcr p15, 0, r0, c1, c0, 0
    // complete all pending memory accesses
    isb
    bx lr
.endfunc

.global disable_icache
.func disable_icache
disable_icache:
    dmb
	// read the system control register (SCTLR)
    mrc p15, 0, r0, c1, c0, 0
    // clear the i bit
    bic r0, r0, #SCTLR_I
    // write changes to system control register (SCTLR)
    mcr p15, 0, r0, c1, c0, 0
    // complete all pending memory accesses
    isb
    bx lr
.endfunc


/* 
 * The following two functions do nothing on ARMv8
 * because the Z-bit isn't present in SCTLR any more. (on ARMv7 it is)
 */
.global enable_branch_predictor
.func enable_branch_predictor
enable_branch_predictor:
    dmb
    // read the system control register (SCTLR)
    mrc	p15, 0, r0, c1, c0, 0
    // set the z bit
    orr r0, r0, #SCTLR_Z
    // write changes to system control register (SCTLR)
    mcr p15, 0, r0, c1, c0, 0
    bx lr
.endfunc

.global disable_branch_predictor
.func disable_branch_predictor
disable_branch_predictor:
    dmb
    // read the system control register (SCTLR)
    mrc	p15, 0, r0, c1, c0, 0
    // clear the z bit
    bic r0, r0, #SCTLR_Z
    // write changes to system control register (SCTLR)
    mcr p15, 0, r0, c1, c0, 0
    bx lr
.endfunc


.global invalidate_icache
.func invalidate_icache
invalidate_icache:
    dmb
    // (ICIALLU) Instruction Cache Invalidate All to PoU
    // The ICIALLU operation ignores the value in r0.
    mcr p15, 0, r0, c7, c5, 0
    isb
    bx lr
.endfunc

.global invalidate_branch_predictor
.func invalidate_branch_predictor
invalidate_branch_predictor:
    dmb
    // (BPIALL) Branch predictor invalidate all
    // The BPIALL operation ignores the value in r0.
    mcr p15, 0, r0, c7, c5, 6
    isb
    dsb
    bx lr
.endfunc


/***********************************************************************
 * Code for D-Cache-Invalidation from Sandro Pinto (Cortex-A9 port) adapted by Michael Zehrer
 **********************************************************************/
/***************** Data Cache disable (L1) ****************/
.global dCache_cleanLine_mva
.func dCache_cleanLine_mva
@ void dCache_cleanLine_mva(uint32_t mva);
clean_dcache_line_mva:
    // (DCCMVAC) Data Cache line Clean by VA to PoC
    mcr	p15, 0, r0, c7, c10, 1
    bx lr
.endfunc

/************** Data Cache invalidate (L1) ***************/
/* 	r0 - (cache level << 1)
	r1 - Log2 Line Length
	r2 - Way
	r3 - LEADING ZEROS of WAY
	r4 - Set
	DO NOT USE r5-r7
*/
dCache_invalidate_setWay:
	mov	r9, r2
1:
	// val = (cache level << 1) | (set << log2 LINE_LEN) |  (way << LEADING_ZEROS_OF WAY)
	orr	r11, r0, r9, lsl r3
	orr	r11, r11, r4, lsl r1
	
	// (DCISW) Data Cache line Invalidate by Set/Way
	mcr 	p15, 0, r11, c7, c6, 2
	subs	r9, r9, #1
	bge 	1b
	subs	r4, r4, #1
	bge 	dCache_invalidate_setWay
	bx      lr

/*  r0 - (cache level << 1) */
invalidate_dcache_level:
    // Save lr (faster than push on stack)
    mov     r7, lr
    
    // Write (CSSELR) Cache Size Selection Register
	mcr     p15, 2, r0, c0, c0, 0
	isb
			
	// Read (CCSIDR) Current Cache Size ID Register
	mrc     p15, 1, r5, c0, c0, 0
    
    // Extract LineSize
	mov32   r6, CCSIDR_LINE_SIZE_MASK
	and     r1, r5, r6
	add     r1, r1, #4
    
	// Extract Associativity
	mov32   r6, CCSIDR_ASSOCIATIVITY_MASK
	and     r2, r5, r6
	lsr     r2, r2, #CCSIDR_ASSOCIATIVITY_START_BIT
	
	clz	    r3, r2
	
	// Extract NumSets
	mov32   r6, CCSIDR_NUM_SETS_MASK
	and     r4, r5, r6
	lsr     r4, r4, #CCSIDR_NUM_SETS_START_BIT
	
	bl	    dCache_invalidate_setWay
	
	// Set the cache size selection back to L1
	mov     r0, #0
	// Write (CSSELR) Cache Size Selection Register
	mcr     p15, 2, r0, c0, c0, 0
	
	// Restore lr from r7
    bx      r7

.global invalidate_dcache
.func invalidate_dcache
invalidate_dcache:
	push	{r4-r12, lr}
	dmb
	
	/* Invalidate Level 1 cache */
	mov     r0, #0
	bl      invalidate_dcache_level
	
	/* Invalidate Level 2 cache */
	mov     r0, #2
	//bl      invalidate_dcache_level
	
	dsb
	isb
	pop	    {r4-r12, lr}
	bx	    lr

.endfunc @ invalidate_dcache

