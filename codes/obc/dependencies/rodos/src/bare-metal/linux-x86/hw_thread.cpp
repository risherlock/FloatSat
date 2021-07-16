#include "context.h"
#include "hw_specific.h"
#include "thread.h"

namespace RODOS {

/**
 *create context on stack and return a pointer to it
 */
long* hwInitContext(long* stack, void* object) {
    long* tos     = stack; /* top of stack */
    *tos--        = (long)object;
    *tos          = 0;
    TContext* ctx = (TContext*)(tos - sizeof(TContext) / sizeof(long));
    ctx->EIP      = (long)(threadStartupWrapper);
    ctx->ESP      = (long)stack;
    __asm__ __volatile__("pushf \n"
                         "\tpop %0"
                         : "=r"(ctx->EFLAGS));
    /* save current floating point context */
    __asm__ __volatile__("fnsave (%0)" ::"r"(ctx->floatingPointContext));
    /*  xprintf("init_context: stack=%08lX ctx=%08lX\n",(long)stack,(long)ctx); */
    return (long*)ctx;
}

void startIdleThread() { }

uintptr_t Thread::getCurrentStackAddr(){
    return reinterpret_cast<uintptr_t>(context);
}

}