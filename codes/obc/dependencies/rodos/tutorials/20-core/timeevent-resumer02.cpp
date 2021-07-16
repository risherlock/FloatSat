#include "rodos.h"

static Application module01("TestTimebeats & events 02");


/** A time event to resume a thread **/

class Resumer : public TimeEvent {
    StaticThread<>* toBeResumed;

  public:
    Resumer() { toBeResumed = 0; }

    void resumeThreadAt(StaticThread<>* who, const int64_t when) {
        toBeResumed = who;
        activateAt(when);
    }

    void handle() {
        if(toBeResumed) toBeResumed->resume();
    }
};
static Resumer resumer;

/** A thread which has to be resumed by some one else */

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        while(1) {
            resumer.resumeThreadAt(this, NOW() + 3 * SECONDS);
            double t0 = SECONDS_NOW();
            PRINTF("Timenow =  %3.9f\n", t0);
            suspendCallerUntil();
        }
    }
};
static TestTimeBeat testTimeBeat;
