
/*
 * @author: Sergio Montenegro
 *
 * @brief Used only for the test suite. Not as flight software!
 *
 * SECONDS_NOW() has a resolution of nanoseconds, to compare
 * the output of two runs can not be used, because the time will never
 * be so exact., then use an approximation only in 1/100 sec
 *
 */

#pragma once
#include <stdint.h>
#include "rodos.h"

int32_t secondsNowCnt = 1;
#define CNT_SECONDS_NOW() (((double)secondsNowCnt++) / 10.0)
#define INT_SECONDS_NOW() (double)((int32_t)SECONDS_NOW())

inline void printErrorLog(void) {
    PRINTF("id  text/name\n");
    PRINTF("---------------------------------\n");
    for(uint32_t i = 0; i < errorLog.index; i++) {
        PRINTF("%6d  %s\n", (int)errorLog.id[i], errorLog.text[i]);
    }
}
