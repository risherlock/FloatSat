
#include "rodos.h"
#include "activity.h"

Activity::Activity(const char* name, int prio, int64_t startAt, int64_t _period) : ListElement(activityList, name) {
    suspendedUntil   = startAt; 
    periode          = _period;
    priority         = prio;
    pseudoThreadLine = 0;
}

List Activity::activityList = 0;

/*******************************************/

class ActivityExecuter : public StaticThread<> {
public:
    ActivityExecuter() : StaticThread<>("ActivityExecuter") { }
    void init();
    void run();
} activityExecuter;

void ActivityExecuter::init() {
    xprintf("\ninit activities");
    ITERATE_LIST(Activity, Activity::activityList) {
        xprintf("\n     %s:", iter->name);
        iter->init();
    }
}

void ActivityExecuter::run(void) {
    while (1) {
        int64_t nextTime = END_OF_TIME;
        int64_t timeNow = NOW();
        Activity* nextActivity = 0;

        ITERATE_LIST(Activity, Activity::activityList) {
            if (iter->suspendedUntil <= timeNow) { // due now
               if (nextActivity == 0) nextActivity = iter;
	       if (iter->priority > nextActivity->priority) nextActivity = iter;
            } 
            if (iter->suspendedUntil < nextTime) nextTime = iter->suspendedUntil;
        } // listierator
        if (nextActivity != 0 ) {
            nextActivity->step(timeNow);
            nextActivity->suspendedUntil = TimeModel::computeNextBeat(nextActivity->suspendedUntil, nextActivity->periode, timeNow);
        }
        timeNow = NOW(); // you may delete this if steps are very fast
        if(nextTime > timeNow) suspendCallerUntil(nextTime);
    } // loop
} // run

