


/**
* @file scheduler.cc
* @date 2010/09/24 18:10
* @author Sergio Montenegro
*
*
* @brief priority based scheduler
*
*/
#include "rodos.h"
#include "scheduler.h"
#include "hw_specific.h"
#include <errno.h>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

namespace RODOS {
/** count all calls to the scheduler */
unsigned long long Scheduler::scheduleCounter = 0;


pthread_mutex_t signal_mutex                  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t interruptDisableCounter_mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int    interruptDisableCounter       = 0;

void sigio_handler(int a);
void timerSignalHandler(int ignore);
void sharedmemorySignalHandler(int signum);

void signalprocessLoop() {
    int      signalnum;
    sigset_t signalsToProcess;
    sigemptyset(&signalsToProcess);
    sigaddset(&signalsToProcess, SIGIO);
    sigaddset(&signalsToProcess, SIGALRM);
    sigaddset(&signalsToProcess, SIGUSR1);


    while(1) {
        if(sigwait(&signalsToProcess, &signalnum) < 0)
            xprintf("sigwait failed, errno: %d\n", errno);

        pthread_mutex_lock(&signal_mutex);
        switch(signalnum) {
            case SIGIO:
                sigio_handler(signalnum);
                break;
            case SIGUSR1:
                sharedmemorySignalHandler(signalnum);
                break;
            case SIGALRM:
                timerSignalHandler(signalnum);
                break;
            default:
                xprintf("Warning: Got unknown signal: %d\n", signalnum);
                break;
        }
        pthread_mutex_unlock(&signal_mutex);
    }
}


void hwDisableInterrupts() {
    pthread_mutex_lock(&interruptDisableCounter_mutex);
    interruptDisableCounter++;
    if(interruptDisableCounter == 1) {
        pthread_mutex_lock(&signal_mutex);
    }
    pthread_mutex_unlock(&interruptDisableCounter_mutex);
}

void hwEnableInterrupts() {
    pthread_mutex_lock(&interruptDisableCounter_mutex);
    interruptDisableCounter--;
    RODOS_ASSERT(interruptDisableCounter >= 0); // called hwEnabeldInterrupts without corresponding hwDisableInterrupt
    
    if(interruptDisableCounter == 0) {
        pthread_mutex_unlock(&signal_mutex);
    }
    pthread_mutex_unlock(&interruptDisableCounter_mutex);
}


void schedulerWrapper([[gnu::unused]] long* ctx) {
}

/** activate idle thread */
void Scheduler::idle() {

    // Disable signals in all Threads
    // We handle signals synchronously in this(idle) thread
    sigset_t newset;
    sigfillset(&newset);
    sigdelset(&newset, SIGINT); // keep CTRL-C working
    pthread_sigmask(SIG_SETMASK, &newset, 0);

    Thread::startAllThreads();
    Timer::start();

    signalprocessLoop();

    while(1) sleep(0xffffffff); // while(1) becouse timerinterups terminaes the sleep
}

// Not used in posix, becouse posix hast its own schedulder
// void Scheduler::schedule() { }

unsigned long long Scheduler::getScheduleCounter() {
    return scheduleCounter;
}

} // namespace RODOS
