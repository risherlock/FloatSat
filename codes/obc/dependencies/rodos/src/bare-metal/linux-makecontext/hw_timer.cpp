

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

#include <ucontext.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
int64_t Timer::microsecondsInterval = 0;


extern "C" {
  /**
  * Declaration of function with C linkage.
  * Is implemented in assembly.
  * Saves the current context and calls the scheduler.
  */
  extern void __asmSaveContext();
}

extern long long timeToTryAgainToSchedule;
extern bool isSchedulingEnabled;
extern void *signal_stack;

/**
* the signal handler for SIGVTALRM (timer signal)
*/
void timerSignalHandler(int, siginfo_t *, void *) {

   if(!isSchedulingEnabled) return;
   long long timeNow = NOW();  // comment this out to improve performance, but: no time events any more
   TimeEvent::propagate(timeNow); // comment this out to improve performance, but: no time events any more

   if(timeNow < timeToTryAgainToSchedule) return;
   __asmSaveContextAndCallScheduler();
}

/**
* initialize the timer and signal handler 
*/
void Timer::init() {
  static int once = 0;
  if (once == 0) {
    int retval;
    struct sigaction action;

    once = 1;
    signal_stack = xmalloc(4096);
    /* callback function for signal handling */
    action.sa_sigaction = timerSignalHandler;

    /* SA_ONSTACK - signal handler should use its own stack */ //<- disabled for the use of makecontext with newlib
    /* SA_RESTART - interrupted system calls shall be restartet */
    action.sa_flags = SA_SIGINFO | SA_RESTART;

    /* empty signal set */
    sigemptyset(&action.sa_mask);

    retval = sigaction(SIGALRM,&action,0);
    RODOS_ASSERT(retval == 0); // error during call to sigaction
  }
}

/**
* start timer 
*/
void Timer::start() {
  itimerval params;
  int retval;
  params.it_interval.tv_sec  = static_cast<__time_t>     (microsecondsInterval / 1000000);
  params.it_interval.tv_usec = static_cast<__suseconds_t>(microsecondsInterval % 1000000);
  params.it_value.tv_sec     = params.it_interval.tv_sec;
  params.it_value.tv_usec    = params.it_interval.tv_usec;
  retval = setitimer(ITIMER_REAL,&params,0);
  RODOS_ASSERT(retval == 0); // error during call to sigaction
}

/**
* stop timer 
*/
void Timer::stop() {
  struct itimerval params;
  int retval;
  params.it_interval.tv_sec = 0;
  params.it_interval.tv_usec = 0;
  params.it_value.tv_sec = 0;
  params.it_value.tv_usec = 0;
  retval = setitimer(ITIMER_REAL,&params,0);
  RODOS_ASSERT(retval == 0); // error during call to sigaction
}

/**
* set timer interval 
*/
void Timer::setInterval(const int64_t microsecondsInterval) {
  Timer::microsecondsInterval = microsecondsInterval;
}

} // namespace

