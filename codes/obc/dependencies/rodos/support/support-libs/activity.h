#pragma once

#include "misc-rodos-funcs.h"

class Activity : public ListElement {
public:
    static List activityList;

    int pseudoThreadLine;

    int64_t suspendedUntil;
    int64_t periode;
    int32_t priority;

    Activity(const char* name = "anonymActiviy", int prio=100, int64_t startAt=END_OF_TIME, int64_t _period=0);
    virtual ~Activity() {
        if(RODOS::isShuttingDown) return;
        RODOS_ERROR("activity deleted");
    }

    void activateAt(const int64_t time) { suspendedUntil = time; }
    void activatePeriodic(const int64_t startAt, const int64_t _periode) {
        periode = _periode;
        suspendedUntil = startAt;
    }

    virtual void init() { }
    virtual void step([[gnu::unused]] int64_t timeNow) { }
};


// inspired by the protothreads from: http://dunkels.com/adam/pt/

#define GOTO_LAST_YIELD  switch(pseudoThreadLine) { case 0:
#define GOTO_LAST_YIELD_END } pseudoThreadLine = 0;

#define YIELD()  pseudoThreadLine =  __LINE__; return ; case __LINE__:
#define YIELD_UNTIL(t) suspendedUntil = t; pseudoThreadLine =  __LINE__; return ; case __LINE__:


