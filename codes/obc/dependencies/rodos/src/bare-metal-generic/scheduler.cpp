

/**
* @file scheduler.cc
* @date 2008/04/24 18:10
* @author Lutz Dittrich
*
* @brief priority based scheduler
*
*/
#include "rodos.h"
#include "scheduler.h"
#include "hw_specific.h"


namespace RODOS {

extern "C" {
  /**
  * A piece of memory used as stack by the scheduler.
  */
  char __schedulerStack__[SCHEDULER_STACKSIZE];

  /**
  * Address of the top of the scheduler's stack.
  * 8-byte aligned
  */
  long __schedulerStackFrame__ =
                  ((long)&(__schedulerStack__[SCHEDULER_STACKSIZE-sizeof(long)])) & (~7);
}

/** count all calls to the scheduler */
unsigned long long Scheduler::scheduleCounter=0;
Thread* Scheduler::preSelectedNextToRun = 0;
long long Scheduler::preSelectedTime = 0;

bool isSchedulingEnabled = true; ///< will be checked before some one calls scheduler::schedule

bool globalAtomarLock()   { isSchedulingEnabled = false; return true; }
bool globalAtomarUnlock() { isSchedulingEnabled = true;  return true; }


void schedulerWrapper(long* ctx) {
  Thread::currentThread->context=ctx;
  Scheduler::schedule();
}

extern Thread* idlethreadP;

/** activate idle thread */
void Scheduler::idle() {
  idlethreadP->suspendedUntil = 0;


  Thread::currentThread = idlethreadP;
  schedulerRunning = true;  /* a bit to early, but no later place possible */

  /* - the order of activate() and startIdleThread() is important -> don't change
   * - For all cortex ports a global context pointer is initialised in activate()
   *   and this must have been done before startIdleThread() is called.
   */
  idlethreadP->activate();

  startIdleThread(); // only for some architectures, most implementations == nop()
  
}

void Scheduler::schedule() {
  Scheduler::scheduleCounter++;

  /** Optimisations: if Thread::yield() prepared time and next to run, use it, but only once! **/
  int64_t timeNow = preSelectedTime;  // Eventually set by Thread::yield()
  if(timeNow == 0) timeNow = NOW(); // Obviously not set, then recompute
  preSelectedTime = 0;              // use only once

  // time events to call?
  // now obsolete! call directly from timer!! TimeEvent::propagate(timeNow);

  /** select the next thread to run: Do we have a preselection from Thread::yield()? **/
  Thread* nextThreadToRun = preSelectedNextToRun; // eventually set by Thread::yield()
  if(nextThreadToRun == 0)  nextThreadToRun = Thread::findNextToRun(timeNow); // not the case
  preSelectedNextToRun = 0;                      // use only once

  // now activate the selected thread
  nextThreadToRun->lastActivation = Scheduler::scheduleCounter; // timeNow ?? but what with on-os_xx, on-posix, etc?
  nextThreadToRun->activate();
}

unsigned long long Scheduler::getScheduleCounter() {
  return scheduleCounter;
}

} // namespace

