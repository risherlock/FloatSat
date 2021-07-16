/** 
* @file asm.s
* @date 2008/04/23 8:51
* @author Lutz Dittrich
*
*
* assembly function for context switching ...
*
*/

.file "asm.S"

.section ".text"

__asmSwitchToContext:
    .globl __asmSwitchToContext
    .type __asmSwitchToContext,@function
    /* void __asmSwitchToContext(long* context) */
    /* abi: 4(%esp) first parameter */
    /*        (%esp) return address  */
    mov 4(%esp), %esp
    frstor (%esp)
    add $108, %esp
    popf
    popa
    ret

__asmSaveContext:
    .globl __asmSaveContext
    .type __asmSaveContext,@function
    /* called by timerSignalHandler */ 

    /* put __interruptedInstructionPointer__ on stack as target for ret */
    push %eax
    push %eax
    mov __interruptedInstructionPointer__, %eax
    mov %eax, 4(%esp)
    pop %eax

1:
    /* save context on stack */
    pusha
    pushf
    sub $108, %esp
    fsave (%esp)
    
    /* clear direction flag  see: System V ABI */
    cld 
    mov %esp, %eax
    mov __schedulerStackFrame__, %esp
    push %eax
    call schedulerWrapper


__asmSaveContextAndCallScheduler:
    .globl __asmSaveContextAndCallScheduler
    .type __asmSaveContextAndCallScheduler,@function
    /* void __asmSaveContextAndCallScheduler() */
    /* called by yield */
    
    /* put return address to stack */
    push %eax
    push %eax
    mov $0f, %eax  /* set address of ret statement (label: 0)*/
    mov %eax, 4(%esp)
    pop %eax
    
    jmp 1b /* reuse the upper code */
0:  ret
