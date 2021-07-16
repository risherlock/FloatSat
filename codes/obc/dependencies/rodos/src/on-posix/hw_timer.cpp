

/**
* @file hw_timer.cc
* @date 2008/04/23 16:36
* @author Lutz Dittrich
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
int64_t Timer::microsecondsInterval = 0;


/**
* the signal handler for SIGVTALRM (timer signal)
*/
void timerSignalHandler(int ignore);
void timerSignalHandler([[gnu::unused]] int ignore) {

    // time events to call?
    int64_t timeNow = NOW();       // comment this out to improve performance, but: no time events any more
    TimeEvent::propagate(timeNow); // comment this out to improve performance, but: no time events any more
}

/**
* initialize the timer and signal handler 
*/
void Timer::init() {
    //Signal handlers are called synchronously from the main thread, see scheduler.cpp
    //signal(SIGALRM, timerSignalHandler);
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
void Timer::setInterval(const int64_t microsecondsInterval) {
    Timer::microsecondsInterval = microsecondsInterval;
}

} // namespace RODOS
