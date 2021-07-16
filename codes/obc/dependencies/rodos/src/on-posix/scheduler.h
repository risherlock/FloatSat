

/**
* @file scheduler.h
* @date 2010/09/24 16:23
* @author Sergio Montenegro
*
*
* @brief priority based %scheduler (header)
*
*/

#pragma once


namespace RODOS {

/**
* @class Scheduler
* @brief A class for the scheduling algorithm.
*
* Priority based scheduler.
*/
class Scheduler {

  private:
    static unsigned long long scheduleCounter;

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


} // namespace RODOS
