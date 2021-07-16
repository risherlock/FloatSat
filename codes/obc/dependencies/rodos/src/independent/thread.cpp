/**
* @file thread.cc
* @date 2008/04/22 16:50
* @author Lutz Dittrich, Sergio Montenegro
*
*
* A Thread is a schedulable object with own context and stack
*
* @brief %Thread handling
*/

#include "rodos-debug.h"
#include "thread.h"
#include "timepoints.h"


namespace RODOS {

/********************************************/

void Thread::setPeriodicBeat(const int64_t begin,
                             const int64_t period) {
    nextBeat = begin;
    this->period = period;
}

void Thread::suspendUntilNextBeat() {
    while (period == 0) {
        RODOS_ERROR("Thead with Period 0 waiting for next beat");
        suspendCallerUntil(); // WRONG! Period shall not be 0 for beats
    }

    suspendCallerUntil(nextBeat);
    int64_t timeNow = NOW();
    nextBeat = TimeModel::computeNextBeat(nextBeat, period, timeNow);
}


/********************************************************/


int32_t Thread::setPrioCurrentRunner(int32_t newPrio) {
    Thread* runner = getCurrentThread();
    if(runner==0) {
        errorLog.addRaw("null pointer setPrioCurrentRunner");
        return 0;
    }
    int32_t previusPriority = runner->getPriority();
    runner->setPriority(newPrio);
    return previusPriority;
}


volatile bool schedulerRunning = false;

}

