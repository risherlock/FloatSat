/**
 * @file interrupts.S
 * @date 2016/09/23
 * @author Michael Zehrer, based on code from Johannes Freitag
 *
 * Copyright 2016 University Wuerzburg
 */
 #include "include/asm_defines.h"

.section ".text"

.globl swint
.globl irq

// hardware interrupt handler:
// 1) pushes the context to the old task stack
// 2) call 'handleInterrupt'-function
// 3) pops the context of the new task from the new task stack
irq:
    //lr has to be adjusted in hardware irq
	sub lr,lr,#4

	/* Step 1: save old context */
	
	// push r0 to use the register
	push {r0}
	// set r0 to point to the task stack pointer
	stmdb sp, {sp}^ // ^ means get the system mode SP value
	sub sp, sp, #4
	pop {r0}
	// push the return address onto the task stack
	stmdb r0!, {lr}
	// now we have saved lr we can use it instead of r0 
	mov lr, r0
	// pop r0 so we can save it onto the task stack
	pop {r0}
	// push all the system mode registers onto the task stack
	stmdb lr, {r0-r14}^
	sub lr, lr, #60
	// push all the fpu mode registers onto the task stack
//    vstmdb lr!, {d0-d15}      // see also lines 101/102 and 128/129!!
//    vstmdb lr!, {d16-d31}
	// Push the SPSR onto the task stack.
	mrs r0, spsr
	stmdb lr!, {r0}
	

	/* Step 2: call handler and sheduler */
	mov r0, lr
	bl handleInterrupt

	/* Step 3: restore context of new task */
	b context_load

// software interrupt handler
swint:
    // push r0 to use the register
    push {r0}
    // calculate address of SWI instruction and load it into r0.
    ldr r0, [lr,#-4]
    // mask off top 8 bits of instruction to give SWI number.
    bic r0, r0, #0xff000000

    cmp r0, #SWI_START_IDLE
    beq start_idle 

    cmp r0, #SWI_CONTEXT_SWITCH
    beq context_switch

    // if no swi number matches then go to the undefined handler
    .word 0xe7f000f0

/** Context Switch
* 1) pushes the context to the old task stack
* 2) schedules (or rather call 'schedulerWrapper')
* 3) pops the context of the new task from the new task stack
*
* context_switch is called from the ISR of the software interrupt
* (when the swi number matches SWI_CONTEXT_SWITCH)
* This software interrupt is raised when Thread::yield has been called.
* In swi the interrupts are disabled*/
context_switch:
    /* Step 1: save old context */

    // r0 is already pushed so we can use the register
	
    // set r0 to point to the task stack pointer
    stmdb sp, {sp}^ // ^ means get the system mode SP value
    sub sp, sp, #4
    pop {r0}
    // push the return address onto the task stack
    stmdb r0!, {lr}
    // now we have saved lr we can use it instead of r0 
    mov lr, r0
    // pop r0 so we can save it onto the task stack
    pop {r0}
    // push all the system mode registers onto the task stack
    stmdb lr, {r0-r14}^
    sub lr, lr, #60
    // push all the fpu mode registers onto the task stack
//    vstmdb lr!, {d0-d15}
//    vstmdb lr!, {d16-d31}
    // Push the SPSR onto the task stack.
    mrs r0, spsr
    stmdb lr!, {r0}


    /* Step 2: call scheduler with top of task stack in r0 */
    mov r0, lr
    bl schedulerWrapper

    /* Step 3: restore context of new task */
    b context_load

start_idle:
    // clean the stack
    pop {r0}
    // start idle is basically the same as context_load
context_load:
    // get the top of stack for the task
    ldr r0, =contextT
    // move the task stack pointer into the Link Register
    ldr lr, [r0]
    // Get the SPSR from the stack.
    ldmfd lr!, {r0}
    msr spsr_cxsf, r0
    // restore all fpu registers
//    vldmia lr!, {d16-d31}
//    vldmia lr!, {d0-d15}
    // restore all system mode registers for the task
    ldmfd lr, {r0-r14}^
    // restore the return address
    ldr lr, [lr, #+60]
    // return to next instruction in new task and leave irq
    movs pc, lr

