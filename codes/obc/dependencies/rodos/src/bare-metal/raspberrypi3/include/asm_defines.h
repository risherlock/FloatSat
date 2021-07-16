/**
 * @file asm_defines.h
 * @date 2016/09/23
 * @author Michael Zehrer
 *
 * @brief defines for assembly code
 *
 * Copyright 2016 University Wuerzburg
 */

#ifndef _ASM_DEFINES_H_
#define _ASM_DEFINES_H_

#ifndef BIT
#    define BIT(n) (1 << (n))
#endif

/***********************************************************************
 * Definitions derived from the Current Program Status Register (CPSR)
 **********************************************************************/
// See ARMv8-A Architecture Reference Manual - Section G1.9.1 (PE Modes)
#define CPSR_MODE_USER 0x10
#define CPSR_MODE_FIQ 0x11
#define CPSR_MODE_IRQ 0x12
#define CPSR_MODE_SUPERVISOR 0x13
#define CPSR_MODE_MONITOR 0x16
#define CPSR_MODE_ABORT 0x17
#define CPSR_MODE_HYPERVISOR 0x1A
#define CPSR_MODE_UNDEFINED 0x1B
#define CPSR_MODE_SYSTEM 0x1F

// See ARMv8-A Architecture Reference Manual - Section G1.9.3 (Program status registers)
#define CPSR_MODE_MASK 0x1F
#define CPSR_IRQ_MASK 0x80
#define CPSR_FIQ_MASK 0x40
#define CPSR_THUMB 0x20

/***********************************************************************
 * Definitions derived from the System Sontrol Register (SCTLR)
 **********************************************************************/
// See ARMv8-A Architecture Reference Manual - Section G4.2.114 (SCTLR)
#define SCTLR_M BIT(0)    // mmu enable
#define SCTLR_C BIT(2)    // data cache enable
#define SCTLR_Z BIT(11)   // branch prediction enable (not on ARMv8)
#define SCTLR_I BIT(12)   // instruction cache enable
#define SCTLR_TRE BIT(28) // TEX remap enable.

/***********************************************************************
 * Definitions derived from the Current Cache Size ID Register (CCSIDR)
 **********************************************************************/
#define CCSIDR_LINE_SIZE_START_BIT 0
#define CCSIDR_LINE_SIZE_MASK 0x7
#define CCSIDR_ASSOCIATIVITY_START_BIT 3
#define CCSIDR_ASSOCIATIVITY_MASK 0x00001FF8
#define CCSIDR_NUM_SETS_START_BIT 13
#define CCSIDR_NUM_SETS_MASK 0x0FFFE000
#define CCSIDR_WA BIT(28) // Write-allocation support
#define CCSIDR_RA BIT(29) // Read-allocation support
#define CCSIDR_WB BIT(30) // Write-back support
#define CCSIDR_WT BIT(31) // Write-through support


/***********************************************************************
 * Software interrups
 **********************************************************************/
// SWI numbers
#define SWI_START_IDLE 0
#define SWI_CONTEXT_SWITCH 1

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


#endif /*_ASM_DEFINES_H_ */
