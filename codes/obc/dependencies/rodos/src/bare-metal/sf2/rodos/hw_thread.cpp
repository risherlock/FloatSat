#include "thread.h"
#include "hw_specific.h"

#include "../CMSIS/m2sxxx.h"

extern volatile long* contextT;

namespace RODOS {

/* Constants required to set up the initial stack. */
constexpr long INITIAL_XPSR = 0x01000000;
constexpr long INITIAL_EXEC_RETURN = static_cast<long>(0xfffffffd);

/**
 * create context on stack and return a pointer to it
 * - very helpful document: "PM0214 Programming manual - STM32F3xxx and STM32F4xxx Cortex-M4 programming manual"
 * - page 42
 */
long* hwInitContext(long* stack, void* object) {

    stack--; // to maintain the initial stack pointer double word aligned
             // we have to decrement the stack pointer by an even number (FA-2012.02
    /* Simulate the stack frame as it would be created by a context switch
	 interrupt. */
    *stack = INITIAL_XPSR; // xPSR
    stack--;
    *stack = (long)(threadStartupWrapper); // PC
    stack--;
    *stack = 0;            // LR
    stack -= 5;            // R12, R3, R2 and R1
    *stack = (long)object; // R0
    stack--;
    *stack = INITIAL_EXEC_RETURN;
    stack -= 8; // R11, R10, R9, R8, R7, R6, R5 and R4

    return stack;
}

void startIdleThread() __attribute__((naked));
void startIdleThread() {
    __asm volatile(
      " ldr r0, =0xE000ED08 	\n" /* Use the NVIC offset register to locate the stack. */
      " ldr r0, [r0] 			\n"
      " ldr r0, [r0] 			\n"
      " msr msp, r0			\n"     /* Set the msp back to the start of the stack. */
      " cpsie i				\n"     /* Globally enable interrupts. */
      " svc 0					\n" /* System call to start first task. */
      " nop					\n");
}

uintptr_t Thread::getCurrentStackAddr(){
    return reinterpret_cast<uintptr_t>(context);
}
}

extern "C" {
void __asmSwitchToContext(long* context) {
    contextT = context;
}

void __asmSaveContextAndCallScheduler() {
    /* Set a PendSV-interrupt to request a context switch. */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}
}