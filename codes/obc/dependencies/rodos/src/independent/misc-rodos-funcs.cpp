
/**
* @file misc.cc
* @date 2008/04/22 17:08
* @author Lutz Dittrich, Sergio Montenegro
*
*
* @brief small support functions
*
*/
#include "default-platform-parameter.h"
#include "hostinfos.h"
#include "rodos-assert.h"
#include "thread.h"

#include "hw_specific.h"


namespace RODOS {

bool isSchedulerRunning() { return schedulerRunning; }

char xmallocBuf[XMALLOC_SIZE] = {0,0,0,0} ;

void* xmalloc(size_t len) {
   static size_t index = 0;

   len = (len+7) & ~0x7u; // round to be 64 Byte aligned

   RODOS_ASSERT_IFNOT_RETURN(len <= XMALLOC_SIZE,        nullptr); // out of memmory?
   RODOS_ASSERT_IFNOT_RETURN(!isSchedulerRunning(),      nullptr); // Xmalloc after system init completation
   RODOS_ASSERT_IFNOT_RETURN(index + len < XMALLOC_SIZE, nullptr); // xmalloc out of mem

   void *allocated =  &xmallocBuf[index];
   index += len;
   return allocated;
}



/******************************************************/

void nop( ... ) { }


/****************************************************/

float getCpuLoad() {
    static int64_t timeLastCall = 0;
    int64_t timeNow = NOW();
    double timeDiff = (double)(timeNow - timeLastCall)/(double)SECONDS;
    timeLastCall = timeNow;

    static int64_t lastIdelCnt  =  0;
    double idleDiff = static_cast<double>(idleCnt - lastIdelCnt);  // idleCnt comes from rodos scheduler
    lastIdelCnt = idleCnt;

    double cpuLoad =  1.0 - (idleDiff/timeDiff) / (double)getMaxIdleCntPerSecond(); 
    if(cpuLoad > 1.0) cpuLoad = 1.0;
    if(cpuLoad < 0.0) cpuLoad = 0.0;
    return static_cast<float>(cpuLoad);
}

}


