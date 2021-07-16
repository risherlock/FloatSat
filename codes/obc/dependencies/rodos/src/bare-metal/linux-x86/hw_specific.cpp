
/**
 * @file hw_specific.cc
 * @date 2008/04/23 7:33
 * @author Sergio Montenegro, Lutz Dittrich
 *
 * Uni Wuerzburg
 *
 * @brief all hardware specific stuff
 *
 */

#include "hw_specific.h"
#include "misc-rodos-funcs.h"
#include "thread.h"
#include "yprintf.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h> // only for nanosleep

namespace RODOS {

/**
 * initialize hardware specific stuff
 */

extern void sigtermHandler(int sig);
extern int32_t myNodeNr;

//_____________________________________________________
void hwInit() {
    schedulerRunning = false;
    signal(SIGTERM, sigtermHandler); // External interrupts simulator
    myNodeNr = getpid();             // a default node number, may be replaced by the link interface (gateway)
}

//_____________________________________________________
void hwResetAndReboot() {
    isShuttingDown = true;
    xprintf("hw_resetAndReboot() -> exit\n");
    exit(0);
}

//_____________________________________________________
// cpu core and periferis off until extern interrupt or time "until"
void deepSleepUntil(int64_t until) {
    int64_t  deltaT = until - NOW();
    timespec deltaTPosix;
    timespec remainingTime;
    deltaTPosix.tv_sec  = static_cast<time_t>(deltaT / SECONDS);
    deltaTPosix.tv_nsec = static_cast<long>(deltaT % SECONDS);

    hwDisableInterrupts();
    nanosleep(&deltaTPosix, &remainingTime);
    hwEnableInterrupts();
}

//_____________________________________________________
sigset_t oldset;
void hwDisableInterrupts() {
    sigset_t newset;
    sigfillset(&newset);
    sigprocmask(SIG_SETMASK, &newset, &oldset);
}
void hwEnableInterrupts() {
    sigprocmask(SIG_SETMASK, &oldset, 0);
}

//_____________________________________________________
static int64_t timeOfLastTriggerWatchdog = 0;
void hwInitWatchdog( [[gnu::unused]] int32_t intervalMilliseconds) {}
void hwTriggerWatchdog() { timeOfLastTriggerWatchdog = NOW(); }

//_____________________________________________________
void sp_partition_yield() { }
void enterSleepMode()     { }
void FFLUSH()             { fflush(stdout); }

} // namespace RODOS
