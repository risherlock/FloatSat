#pragma once
#include "stdint.h"


/*
This is done only to be able to perform timing anylse with
tools/timing-display/
*/

namespace Common {

struct ThreadTiming {
     static uint16_t threadTimingListCnt;
     int64_t     startTime;
     int64_t     period;
     int64_t     length;
     const char* name;

     ThreadTiming(int64_t st, int64_t per, int64_t len, const char* nm);

     bool runningAt(int64_t time) const { return  ( time >= startTime ) && (((time - startTime) % period) < length);};
};

}

