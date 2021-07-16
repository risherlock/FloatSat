/**
* @file timeevents.cc
* @date 2008/06/11 16:07
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief test of time event management
*/

#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;    //Will be set to 1 in init of TimeEventTest3 to get the PRINTF then to 0 and the in the handle functions to 1

static Application module01("TestTimeEvents");

class TimeEventTest : public TimeEvent {
  protected:
    void handle() {
        printfMask = 1;
        xprintf("SINGLE Time Event at %3.9f\n", CNT_SECONDS_NOW());
    }

    void init() {
        activateAt(25 * MILLISECONDS);
    }
};

static TimeEventTest te01;

class TimeEventTestPeriodic : public TimeEvent {
  public:
    void handle() {
        printfMask = 1;
        xprintf("Periodic Time Event at %3.9f\n", CNT_SECONDS_NOW());
    }

    void init() {
        activatePeriodic(40 * MILLISECONDS, 10 * MILLISECONDS);
    }
};

static TimeEventTestPeriodic te02;

class TimeEventTest3 : public TimeEvent {
    int i = 0;
  protected:
    void handle() {
        printfMask = 1;
        i++;
        if (i > 10) {
            PRINTF("Terminating\n");
            FFLUSH();
            hwResetAndReboot();
        }
        xprintf("Activating Time Event at %3.9f, reactivation in 0.02 seconds\n", CNT_SECONDS_NOW());
        activateAt(NOW() + 20 * MILLISECONDS);
    }

    void init() {
        printfMask = 1;
        xprintf("activateAt(NOW() + 12*MILLISECONDS) now=%3.9f\n", CNT_SECONDS_NOW());
        printfMask = 0;
        activateAt(NOW() + 12 * MILLISECONDS);
    }
};

static TimeEventTest3 te03;
