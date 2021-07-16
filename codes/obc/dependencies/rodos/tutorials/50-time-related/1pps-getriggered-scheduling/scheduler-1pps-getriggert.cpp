
#include "rodos.h"
#include "activity1pps.h"


List Activity1PPS::activityList = 0;

/*******************************************/

class ActivityExecuter : public StaticThread<> {
  public:
    ActivityExecuter() : StaticThread<>("ActivityExecuter") {}
    void init();
    void run();
} activityExecuter;

void ActivityExecuter::init() {
    xprintf("init activities");
    ITERATE_LIST(Activity1PPS, Activity1PPS::activityList) {
        xprintf("\n     %s:", iter->name);
        iter->init();
    }
}


void ActivityExecuter::run(void) {
    while(1) {

        PRINTF("Wating 1PPS, eg on other term type killall tst\n");
        suspendCallerUntil(); // wating for the 1pps

        int64_t startTime = NOW();
        ITERATE_LIST(Activity1PPS, Activity1PPS::activityList) {
            iter->suspendedUntil = startTime + iter->offsetFrom1PPs;
        }

        for(bool inTime = true; inTime;) {
            int64_t nextTime = END_OF_TIME;
            int64_t timeNow  = NOW();

            ITERATE_LIST(Activity1PPS, Activity1PPS::activityList) {

                if(iter->suspendedUntil <= timeNow) {
                    if(iter->frequency == 0) {
                        iter->suspendedUntil = END_OF_TIME;
                    } else {
                        iter->suspendedUntil += SECONDS / iter->frequency;
                    }
                    if(iter->associatedThread != 0) iter->associatedThread->resume();
                    iter->step(timeNow); // User defined
                    timeNow = NOW();     // you may delete this if steps are fast
                } // if due
                if (iter->suspendedUntil < nextTime) nextTime = iter->suspendedUntil;
            } // listierator
            // My clock may differ up to 50 PPM from the 1PPS generator -> 100 Micros as safety range
            inTime = (nextTime <= startTime + SECONDS - 100*MICROSECONDS);  // 100*micros? See last commet
            if(inTime) suspendCallerUntil(nextTime);
        } // loop
    }
} // run


/*****************************************/

class OnePulsePerSecond : public Subscriber {
  public:
    OnePulsePerSecond() : Subscriber(interruptSigterm, "simplesub") {}
    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const void* any, size_t len) {
        xprintf("external Event at %3.9f datalen %lu\n", SECONDS_NOW(), static_cast<unsigned long>(len));
        activityExecuter.resume();
    }
} onePulsePerSecond;
