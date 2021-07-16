
/**
 * @file timepoints.h
 * @date 2008/06/12
 * @author Sergio Montenegro
 *
 * @brief Time tagged log of events
 *
 */

#pragma once
#include <stdint.h>

#include "rodos-debug.h"
#include "thread.h"

namespace RODOS {

/**
 * @class TimePoints
 * @brief %Time tagged text
 *
 * TimePoints registers timepoints associated with a free text comment.
 * This is useful to trace applications and system behavior.
*/

template <size_t POINTS>
class TimePoints {
public:
    int64_t      time[POINTS];
    const char*  text[POINTS];
    int32_t      id[POINTS];  ///< and Id you wish for more infos than just the text line
    uint32_t     index;       ///< how many entries are written (add counter)
    uint64_t     addCounter;  ///< total number of add, ignoring clears

    TimePoints(void) { index = 0; addCounter = 0; }
    void clear(void) { index = 0; }

    bool addRaw(const char* const name, int32_t _id = 0) { // not thread safe
        time[index] = NOW();
        text[index] = name; // ony the pointer which shall be a constant
        id[index]   = _id;
        if (index >= (POINTS -1)) return false; //yes! first write, then ask! Overwrite the last one with intention!
        index++;
        return true;
    }

    bool add(const char* const name, int32_t _id = 0) { // thread safe
        PRIORITY_CEILER_IN_SCOPE();
        return addRaw(name, _id);
    }

    bool isEmpty() { return index==0; }

    void printAll(void) {  ///< prints and clear
        PRINTF("time(secs)         id  text/name\n");
        PRINTF("---------------------------------\n");
        for (uint32_t i = 0; i < index; i++)  {
            PRINTF("%3.9f  %6d  %s\n", (double)time[i]/SECONDS, (int)id[i], text[i]);
        }
    }

};

extern TimePoints<20>  errorLog; ///< system error log for all applications and OS. Shall be empty!

}   // namespace

