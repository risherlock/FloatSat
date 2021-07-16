
/**
* @file hostinfos.h
* @author Sergio Montenegro
*/

#pragma once


#include "stdint.h"


/**  bigendian is standard net protocol. 
 * Warning: Will be set in main.
 * never use before main (in static constructors)
 **/


namespace RODOS {

extern bool  getIsHostBigEndian();          ///< never use before main()
extern int32_t  getSpeedKiloLoopsPerSecond();  ///< only an approximation, max differ +/- 50%
extern int32_t  getMaxIdleCntPerSecond();      // see rodos-tutorials/development-tests/cpuspeedtest.cpp
extern int32_t  getYieldTimeOverhead();        ///< number of nanoseconds for a thread switch
extern const char* getHostCpuArch(); 
extern const char* getHostBasisOS();

extern volatile int64_t idleCnt; ///< a hint about cpu idle time, cnt will be incremented by idleThread

} // namespace

