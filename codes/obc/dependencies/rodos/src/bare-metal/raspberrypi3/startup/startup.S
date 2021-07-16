/**
 * @file startup.S
 * @date 2016/09/23
 * @author Michael Zehrer
 *
 * Copyright 2016 University Wuerzburg
 */
#include "../include/asm_defines.h"

.section ".text.startup"

.globl enable_dcache
.globl enable_icache

.globl disable_dcache
.globl disable_icache

.globl reset
.globl undefined
.globl hang

.globl PUT32
.globl GET32

reset:
    // Disable FIQ and IRQ
    cpsid if

    // Get the current PE mode into r0...
    mrs r0, cpsr
    and r0, r0, #CPSR_MODE_MASK

    // ... and check whether it is in hypervisor mode.
    cmp r0, #CPSR_MODE_HYPERVISOR
    beq reset_hyp_mode

    // If not, go into supervisor mode
    cps #CPSR_MODE_SUPERVISOR
    b reset_svc_mode

reset_hyp_mode:    
    // TODO: HYP stub to allow HVC call

    ldr r0, =reset
    msr ELR_hyp, r0
    mrs r0, cpsr_all

    bic r0, r0, #CPSR_MODE_MASK
    orr r0, r0, #CPSR_MODE_SUPERVISOR
    msr SPSR_hyp, r0
    eret

reset_svc_mode:
    // get the value from Vector Base Address Register (VBAR) 
    mrc p15, 0, r1, c12, c0, 0

    // copy 0x40 bytes from 0x8000 to the address stored in VBAR (typically 0x0000)
    mov r0,#0x8000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
      
    // set the stackpointer for the supervisor mode
    mov sp,#0x5000

    // set the stackpointer for the irq mode
    cps #CPSR_MODE_IRQ
    mov sp,#0x4000

    // set the stackpointer for the system mode
    cps #CPSR_MODE_SYSTEM
    mov sp,#0x8000

    bl disable_dcache
    bl disable_icache
    bl disable_branch_predictor
    
    bl invalidate_dcache
    bl invalidate_icache
    bl invalidate_branch_predictor

/*
    // enable NEON/VFP
    // (based on the code from http://processors.wiki.ti.com/index.php/Cortex-A8)

    // r1 = Architectural Feature Access Control Register
    mrc p15, #0, r1, c1, c0, #2
    // enable full access for cp10 and cp11
    orr r1, r1, #(0xf << 20)
    // Architectural Feature Access Control Register = r1
    mcr p15, #0, r1, c1, c0, #2
    mov r1, #0
    // flush prefetch buffer because of FMXR below
    isb // mcr p15, #0, r1, c7, c5, #4
    // and CP 10 & 11 were only just enabled
    // Enable VFP itself (set bit 30 of FPEXC)
    mov r0, #0x40000000
    // FPEXC = r0
    fmxr fpexc, r0
*/

    // Set the bss-Section to zero
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0

bss_clear_loop:
    // Write 4 bytes at a time (this is possible because the bss-Section is ALIGN(4))
    stmia r0!, {r2-r5}
    // Continue until r0 isn't lower than r1 any more
    cmp r0, r1
    blo bss_clear_loop

    // Call all static constructors
    bl call_constructors

    // Call main
    bl main

    b hang

hang:
    b hang

undefined:
    b hang

;@ write to memory
PUT32:
    str r1,[r0]
    bx lr

;@ read from memory
GET32:
    ldr r0,[r0]
    bx lr

;@ read cpsr
.globl GETcpsr
GETcpsr:
    mrs r0,cpsr
    bx lr

;@ read lr
.globl GETlr
GETlr:
    mov r1,lr
    pop {lr}
    push {lr}
    mov r0,lr
    bx r1

.globl dummy
dummy:
    bx lr

