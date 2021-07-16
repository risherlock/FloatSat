/**
 * @file mmu.c
 * @date 2016/10/24
 * @author Michael Zehrer
 *
 */
#include <stdint.h>
#include <stdbool.h>

#include "include/bcm2837.h"
#include "include/mmu.h"
#include "include/asm_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

// defined by the linker script
extern uint32_t _mmu_level1_table_start_[4096 * 4];

void enable_mmu() {
    __asm volatile(
      // (TLBIALL) TLB Invalidate All entries (value in r0 is ignored)
      "mcr p15, 0, r0, c8, c7, 0\n\t"
      // read the System Control Register (SCTLR, page 4005 - ARMv8 Reference Manual)
      "mrc p15, 0, r0, c1, c0, 0\n\t"
      // ... set the M-bit ...
      "orr r0, r0, #" STR(SCTLR_M) "\n\t"
                                   // ... and write it back
                                   "mcr p15, 0, r0, c1, c0, 0\n\t"
      :
      :
      : "r0", "memory");
}

void disable_mmu() {
    __asm volatile(
      // read the System Control Register (SCTLR, page 4005 - ARMv8 Reference Manual)
      "mrc p15, 0, r0, c1, c0, 0\n\t"
      // ... clear the M-bit ...
      "bic r0, r0, #" STR(SCTLR_M) "\n\t"
                                   // ... and write it back
                                   "mcr p15, 0, r0, c1, c0, 0\n\t"
      :
      :
      : "r0", "memory");
}

void createPageTableEntries(uint32_t physicalAdr, uint32_t virtualAdr, uint32_t length, bool cacheable) {
    mmu_level1_section_t section;
    section.value      = 0;
    section.bits._zero = 0;
    section.bits._one  = 1;
    if(cacheable) {
        // If TEX remap is disabled (should be) this bits stand for
        // Outer and Inner Write-Back, no Write-Allocate (page 3619 - ARMv8 Reference Manual)
        section.bits.b = 1;
        section.bits.c = 1;
    }

    // Only the top 12 bits are needed as base addresses
    uint32_t virtualBase  = virtualAdr >> 20;
    uint32_t physicalBase = physicalAdr >> 20;
    uint32_t entries      = length >> 20;

    for(; entries > 0; ++physicalBase, ++virtualBase, --entries) {
        section.bits.base_address             = physicalBase & 0x0FFFu;
        _mmu_level1_table_start_[virtualBase] = section.value;
    }

    // (TLBIALL) TLB Invalidate All entries (value in r0 is ignored)
    __asm volatile("mcr p15, 0, r0, c8, c7, 0\n\t");
}

void init_mmu() {
    __asm volatile(
      "mvn r0, #0\n\t"
      // (DACR) Domain Access Control Register
      // Set 11 to each domain
      // 11 = Manager. Accesses are not checked against the permission bits in the translation tables.
      "mcr p15, 0, r0, c3, c0, 0\n\t"
      // Load and set the page table address
      "ldr r0, =_mmu_level1_table_start_\n\t"
      // (TTBR0) Translation Table Base Register 0
      "mcr p15, 0, r0, c2, c0, 0\n\t"
      // (TTBR1) Translation Table Base Register 1
      "mcr p15, 0, r0, c2, c0, 1\n\t"
      :
      :
      : "r0", "memory");

    // The complete RAM (from 0 to PERIPHERALS_BASE) could be cached
    createPageTableEntries(0x00000000, 0x00000000, PERIPHERALS_BASE, true);

    // and do not cache the peripherals
    createPageTableEntries(PERIPHERALS_BASE, PERIPHERALS_BASE, PERIPHERALS_SIZE, false);
    createPageTableEntries(ARM_LOCAL_BASE, ARM_LOCAL_BASE, 0x00100000, false);

    enable_mmu();
}

#ifdef __cplusplus
} // end extern "C"
#endif
