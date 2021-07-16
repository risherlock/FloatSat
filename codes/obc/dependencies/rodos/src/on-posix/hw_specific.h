

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

#pragma once

#include <stdint.h>

namespace RODOS {

/***********************************/

void hwInit(); ///< initilisation of all hw interfaces, fpgas, devices, etc.

/** get time, time unit is nanoseconds, time 0 = startup */
int64_t hwGetNanoseconds();
int64_t hwGetAbsoluteNanoseconds(); ///< from host, eg for random generators
void    hwInitTime();               ///< Initialize the time (eg. time 0)

void hwResetAndReboot(); ///<  End of Programm,

/** Allows another Space/Time partition to work, eg
 ** another linux process in linux, or another partition
 ** in an ARIC OS. If standalone then just nop()
 **/
void sp_partition_yield();


/***********************************/


class Thread;
extern void threadStartupWrapper(Thread*);


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


} // namespace RODOS
