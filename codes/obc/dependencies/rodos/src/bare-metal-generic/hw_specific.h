

/**
* @file hw_specific.h
* @date 2008/04/22 16:54
* @author Lutz Dittrich
*
*
* @brief all hardware specific stuff
*
* Each new hw platform shall provide this interface
*
*/

#include <stdint.h>

#pragma once

extern "C" {

  /**
  * Declaration of a function with C linkage.
  * Is implemented in assembly.
  * Loads the given context and continues with its execution.
  */
  extern void __asmSwitchToContext(long* context);

  /**
  * Declaration of a function with C linkage.
  * Is implemented in assembly.
  * Saves the current context and calls the scheduler.
  */
  extern void __asmSaveContextAndCallScheduler();
  
#ifdef __cplusplus
  }
#endif




namespace RODOS {

/***********************************/
void startIdleThread(); //start idle thread for different barmetal implementations

void hwInit();	///< initilisation of all hw interfaces, fpgas, devices, etc.

/** get time, time unit is nanoseconds, time 0 = startup */
int64_t hwGetNanoseconds();
int64_t hwGetAbsoluteNanoseconds();	///< from host, eg for random generators
void hwInitTime();	         ///< Initialize the time (eg. time 0)

void hwResetAndReboot();	///<  End of Programm,

/** Allows another Space/Time partition to work, eg
 ** another linux process in linux, or another partition
 ** in an ARIC OS. If standalone then just nop()
 **/
void sp_partition_yield();

/** Go into sleep mode.
 ** The next schedule timer should wake the hardware.
 **/
void enterSleepMode();


/***********************************/


class Thread;
extern void threadStartupWrapper(Thread* thread);


/** create context on stack and return a pointer to it */
long* hwInitContext(long* stack, void* object);

/** Node number if a network system **/
extern int32_t getNodeNumber();

/********************************************************/

/**
* @class Timer
* @brief Class for interval timer management.
*
*/
class Timer {

private:
  static int64_t microsecondsInterval;
  Timer();

public:
  /**
  * Setup signal handlers for timer signal management.
  */
  static void init();

  ~Timer();

  /**
  * Start the timer signal generation.
  */
  static void start();

  /**
  * Stop the timer signal generation.
  */
  static void stop();

  /**
  *  Set timer interval.
  */
  static void setInterval(const int64_t microsecondsInterval);

  /**
  *  Get timer interval.
  */
  static int64_t getInterval() { return microsecondsInterval; }
};


} // namespace

