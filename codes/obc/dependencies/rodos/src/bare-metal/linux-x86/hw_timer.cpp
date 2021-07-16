

/**
* @file hw_timer.cc
* @date 2008/04/23 16:36
* @author Sergio Montenegro
*
*
* @brief fixed interval timer
*
* class for fixed-interval timer  for Linux as guest os
* uses the virtual timer (counts only when process is executed)
*/

// #include <stdio.h>
// #include <stdlib.h>

#include <signal.h>

#include "hw_specific.h"
#include "rodos.h"
#include <sys/time.h>


/**
* The instruction pointer during the last interruption.
*/
volatile long __interruptedInstructionPointer__ = 0;

namespace RODOS {

/**
* Size of stack required by signal handler.
*/
#define SIGNAL_HANDLER_STACK_SIZE SIGSTKSZ

/** the timer interval */
long long Timer::microsecondsInterval = 0;


extern "C" {
 /**
  * Declaration of function with C linkage.
  * Is implemented in assembly.
  * Saves the current context and calls the scheduler.
  */
extern void __asmSaveContext();
}

extern long long timeToTryAgainToSchedule;
extern bool      isSchedulingEnabled;

/**
* the signal handler for SIGVTALRM (timer signal)
*/
void timerSignalHandler(int ignore);
void timerSignalHandler([[gnu::unused]] int ignore) {

    if(!isSchedulingEnabled) return;
    long long timeNow = NOW();     // comment this out to improve performance, but: no time events any more
    TimeEvent::propagate(timeNow); // comment this out to improve performance, but: no time events any more

    if(timeNow < timeToTryAgainToSchedule) return;


    long* ebp;
    __asm__ __volatile__ ("mov %%ebp, %0":"=r"(ebp));
  
    schedulerRunning = false;

  /* instruction pointer of interrupted programm is at ebp[17] 
  * see System V abi and linux src : include/asm-x86/sigcontext32.h 
  */

  /* we replace the instruction pointer in the saved context and continue with 
  * our own context saving procedure 
  */
    __interruptedInstructionPointer__ = ebp[17];
    ebp[17]=(long)__asmSaveContext;
}

/**
* initialize the timer and signal handler 
*/
char signalHandlerStack[SIGNAL_HANDLER_STACK_SIZE];
void Timer::init() {
    static int once = 0;
    if(once == 0) {
        int              retval;
        struct sigaction action;
        stack_t          signalStack;

        once = 1;

        /* callback function for signal handling */
        action.sa_handler = timerSignalHandler;

        /* SA_ONSTACK - signal handler should use its own stack */
        /* SA_RESTART - interrupted system calls shall be restartet */
        action.sa_flags = SA_ONSTACK | SA_RESTART;

        /* empty signal set */
        sigemptyset(&action.sa_mask);

        signalStack.ss_sp    = signalHandlerStack;
        signalStack.ss_flags = 0;
        signalStack.ss_size  = SIGNAL_HANDLER_STACK_SIZE;
        retval               = sigaltstack(&signalStack, 0);
        RODOS_ASSERT(retval == 0); // error during call to sigaltstack

        //retval = sigaction(SIGVTALRM,&action,0);
        retval = sigaction(SIGALRM, &action, 0);
        RODOS_ASSERT(retval == 0); // error during call to sigaltstack
    }
}

/**
* start timer 
*/
void Timer::start() {
    itimerval params;
    int       retval;
    params.it_interval.tv_sec  = static_cast<time_t>(microsecondsInterval / 1000000);
    params.it_interval.tv_usec = static_cast<suseconds_t>(microsecondsInterval % 1000000);
    params.it_value.tv_sec     = params.it_interval.tv_sec;
    params.it_value.tv_usec    = params.it_interval.tv_usec;
    retval                     = setitimer(ITIMER_REAL, &params, 0);
    RODOS_ASSERT(retval == 0); // error during call to setitimer
}

/**
* stop timer 
*/
void Timer::stop() {
    struct itimerval params;
    int              retval;
    params.it_interval.tv_sec  = 0;
    params.it_interval.tv_usec = 0;
    params.it_value.tv_sec     = 0;
    params.it_value.tv_usec    = 0;
    retval                     = setitimer(ITIMER_REAL, &params, 0);
    RODOS_ASSERT(retval == 0); // error during call to setitimer
}

/**
* set timer interval 
*/
void Timer::setInterval(const long long microsecondsInterval) {
    Timer::microsecondsInterval = microsecondsInterval;
}

} // namespace RODOS
