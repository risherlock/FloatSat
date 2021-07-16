#include "thread.h"
#include "hw_specific.h"
#include "include/asm_defines.h"
#include "include/bcm2837.h"

extern volatile long* contextT;

namespace RODOS {

/**
 * create context on the stack and return a pointer to it
 *
 * this context has to look like it had been created by the contextSwitch
 */
long int* hwInitContext(long* stack, void* object) {
    uint32_t psp = (uint32_t)stack;

    //bottom of stack
    *stack = (int32_t)(threadStartupWrapper); // lr in swi mode
    stack--;
    *stack = 0; // lr in sys mode
    stack--;
    *stack = static_cast<long int>(psp); // sp
    stack--;
    *stack = 0; // r12
    stack--;
    *stack = 0; // r11
    stack--;
    *stack = 0; // r10
    stack--;
    *stack = 0; // r9
    stack--;
    *stack = 0; // r8
    stack--;
    *stack = 0; // r7
    stack--;
    *stack = 0; // r6
    stack--;
    *stack = 0; // r5
    stack--;
    *stack = 0; // r4
    stack--;
    *stack = 0; // r3
    stack--;
    *stack = 0; // r2
    stack--;
    *stack = 0; // r1
    stack--;
    *stack = (int32_t)object; // r0
                              //stack -= 64;                // FPU: VFPv4-D32
    stack--;
    *stack = static_cast<long int>(GETcpsr());      // SPSR
    *stack = *stack & ~0x80; //interrupts enabled
    //top of stack

    return stack;
}

/** switches to the context of the initial idle thread
 * called in main() -> Scheduler::idle();
 * basically the same as "restore context of new task"
 */
__attribute__((naked)) void startIdleThread() {
    __asm volatile("swi " STR(SWI_START_IDLE) "\n\t");
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
    __asm volatile("swi " STR(SWI_CONTEXT_SWITCH) "\n\t");
}
}