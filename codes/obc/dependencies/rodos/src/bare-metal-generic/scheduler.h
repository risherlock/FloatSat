
/**
* @file scheduler.h
* @date 2008/04/24 16:23
* @author Lutz Dittrich
*
*
* @brief priority based %scheduler (header)
*
*/

#pragma once


#include "thread.h"


namespace RODOS {

extern "C" {
  /**
  * This is a wrapper function with C linkage.
  * It stores the context and calls the scheduler.
  */
  void schedulerWrapper(long* ctx);
}

/**
* @class Scheduler
* @brief A class for the scheduling algorithm.
*
* Priority based scheduler.
*/
class Scheduler {

friend class Thread;
private:
  static unsigned long long scheduleCounter;
  static Thread* preSelectedNextToRun;  // used only to optimese yield time
  static long long  preSelectedTime;    // used only to optimese yield time

public:
  /**
  * Call the scheduling algorithm.
  */
  static void schedule();

  /**
  * Activate the idle thread.
  */
  static void idle();

  /**
  *  return schedule_counter
  */
  static unsigned long long getScheduleCounter();

};


} // namespace

