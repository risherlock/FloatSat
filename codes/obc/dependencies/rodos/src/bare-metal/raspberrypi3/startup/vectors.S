/**
 * @file vector.S
 * @date 2016/08/26
 * @author Johannes Freitag, David Welch
 */

.section ".vectors"

.globl _exception_table
_exception_table:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word undefined
swi_handler:        .word swint
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word irq
fiq_handler:        .word hang
