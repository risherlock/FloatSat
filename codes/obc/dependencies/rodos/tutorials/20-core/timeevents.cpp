/**
* @file timeevents.cc
* @date 2008/06/11 16:07
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief test of time event management
*/

#include "rodos.h"

static Application module01("TestTimeEvents");

class TimeEventTest : public TimeEvent {
  protected:
    void handle() {
        xprintf("SINGLE Time Event at %3.9f\n", SECONDS_NOW());
    }

    void init() {
        activateAt(5125 * MILLISECONDS);
    }
};

static TimeEventTest te01;

class TimeEventTestPeriodic : public TimeEvent {
  public:
    void handle() {
        xprintf("Periodic Time Event at %3.9f\n", SECONDS_NOW());
    }

    void init() {
        activatePeriodic(4 * SECONDS, 500 * MILLISECONDS);
    }
};

static TimeEventTestPeriodic te02;

class TimeEventTest3 : public TimeEvent {
  protected:
    void handle() {
        xprintf("activateIn Time Event at %3.9f, reactivation in 2 seconds\n", SECONDS_NOW());
        activateAt(NOW() + 2 * SECONDS);
    }

    void init() {
        xprintf("activateIn( 1234*MILLISECONDS) now=%3.9f\n", SECONDS_NOW());
        activateAt(NOW() + 1234 * MILLISECONDS);
    }
};

static TimeEventTest3 te03;
