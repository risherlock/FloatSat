/**
* @file event.cc
* @date 2008/05/06 10:12
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief %Event management
*/
#include <stdint.h>


#include "listelement.h"
#include "rodos-debug.h"
#include "timeevent.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

/* constructor */
TimeEvent::TimeEvent(const char* name) : ListElement(TimeEvent::timeEventList,name) {
    eventAt     = END_OF_TIME;
    eventPeriod = 0; 
}


/* destructor */
TimeEvent::~TimeEvent() {
    if(isShuttingDown) return;
    RODOS_ERROR("Time EventHandler deleted");
}

/* Sets the time when the handler should be called
 * @param absolute time of next event
*/
void TimeEvent::activateAt(const int64_t time) {
    eventAt = time;
    eventPeriod = 0;
}

/* defines the time relative to now, when the handler should be called: DEPRECATED */
// void TimeEvent::activateIn(const int64_t time) { activateAt(NOW() + time); }

/* like activateAt but with periodic reactivation */
void TimeEvent::activatePeriodic(const int64_t startAt, const int64_t period) {
    eventPeriod = period;
    eventAt = startAt;
}


/** TBA   Invoke event handler. Events are simply invoked by comparing event time and system time.
* calls all time event handlers which eventAt < now (past)
* and updates eventAt.
* Returns the number of handle() called
*/
int32_t TimeEvent::propagate(const int64_t timeNow) {
    int32_t cnt = 0;
    ITERATE_LIST(TimeEvent, TimeEvent::timeEventList) {
        if (iter->eventAt < timeNow) {
            if (iter->eventPeriod == 0) { // not again until user sets it again
                iter->eventAt = END_OF_TIME;
            } else {
                iter->eventAt+= iter->eventPeriod;
                if (iter->eventAt < timeNow) { // Still in the past?
                    iter->eventAt = TimeModel::computeNextBeat(iter->eventAt,
                                    iter->eventPeriod,
                                    timeNow);
                }
            }
            iter->handle();
            cnt++;
        }
    }
    return cnt;
}


/* call init for each element in list */
int32_t TimeEvent::initAllElements() {
    int32_t cnt = 0;
    ITERATE_LIST(TimeEvent, TimeEvent::timeEventList) {
        iter->init();
        cnt++;
    }
    return cnt;
}

}

