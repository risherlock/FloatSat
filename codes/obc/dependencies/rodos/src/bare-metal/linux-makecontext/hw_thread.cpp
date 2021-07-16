#include "hw_specific.h"
#include "misc-rodos-funcs.h"
#include "thread.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

namespace RODOS {

ucontext_t* volatile contextT;

constexpr size_t STACKSIZE = 4096;     /* stack size  WARNING/TODO: That is this? */
ucontext_t signal_context; /* the interrupt context */
extern void* signal_stack;

uintptr_t Thread::getCurrentStackAddr(){
    volatile ucontext_t* c = reinterpret_cast<volatile ucontext_t*>(context);
    return static_cast<uintptr_t>(c->uc_mcontext.gregs[REG_ESP]);
}

/**
 *create context on stack and return a pointer to it
 */
long* hwInitContext(long* stack, void* object) {
    contextT = (ucontext_t*)xmalloc(sizeof(ucontext_t));

    getcontext(contextT);
    contextT->uc_stack.ss_sp    = stack;
    contextT->uc_stack.ss_size  = sizeof stack;
    contextT->uc_stack.ss_flags = 0;
    if(sigemptyset(&contextT->uc_sigmask) < 0) {
        perror("sigemptyset");
        isShuttingDown = true;
        exit(1);
    }
    makecontext(contextT, (void (*)())threadStartupWrapper, 1, object);

    return reinterpret_cast<long*>(contextT);
}

void startIdleThread() { }

}

//___________________________________________________________________________
/*
 *  In other bare metal implementations this has to be done in assembler 
 */
extern "C" void __asmSaveContextAndCallScheduler();
void __asmSaveContextAndCallScheduler() {

    /* Create new scheduler context */
    getcontext(&RODOS::signal_context);
    RODOS::signal_context.uc_stack.ss_sp    = RODOS::signal_stack;
    RODOS::signal_context.uc_stack.ss_size  = RODOS::STACKSIZE;
    RODOS::signal_context.uc_stack.ss_flags = 0;
    sigemptyset(&RODOS::signal_context.uc_sigmask);
    makecontext(&RODOS::signal_context, (void (*)())RODOS::schedulerWrapper, 1, RODOS::contextT);

    /* save running thread, jump to scheduler */
    swapcontext(RODOS::contextT, &RODOS::signal_context);
}

extern "C" void __asmSwitchToContext(long* context);
void __asmSwitchToContext(long* context) {
    RODOS::contextT = (ucontext_t*)context;
    setcontext(RODOS::contextT); /* go */
}
