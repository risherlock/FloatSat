
/**
* @file hw_time.cc
* @date 2008/06/04 10:19
* @author  Sergio Montenegro
*
*
* @brief functions for time management
*/

#include <time.h>
#include <stdint.h>
#include "hw_specific.h"

namespace RODOS {

/***************** Get Time functions **************/

// static long long timeAtStartup = 0LL;  now defined at 00globalobjects
extern int64_t timeAtStartup;

int64_t hwGetNanoseconds() {
    struct timespec tp;
    int64_t       timeNow;

    clock_gettime(CLOCK_REALTIME, &tp);
    timeNow = tp.tv_sec * 1000000000LL + tp.tv_nsec - timeAtStartup;
    return timeNow;
}

void hwInitTime() {
    timeAtStartup = hwGetNanoseconds();
}

/******* abslute time (from host) for exotic function eg.random generator **/

int64_t hwGetAbsoluteNanoseconds() {
    return hwGetNanoseconds() + timeAtStartup;
}

} // namespace RODOS
