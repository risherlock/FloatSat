#include "rodos.h"
#include "activity.h"

class A3 : public Activity {
public:
    A3() : Activity("A3") { }
    void init() { activatePeriodic(10*SECONDS, 10*SECONDS); }
    void step(int64_t timeNow); 
} a3;

void A3::step([[gnu::unused]] int64_t timeNow) {
    GOTO_LAST_YIELD;
    PRINTF("A3.1\n");
    YIELD_UNTIL(NOW() + 1*SECONDS);
    PRINTF("A3.2\n");
    YIELD_UNTIL(NOW() + 1*SECONDS);
    PRINTF("A3.3\n");
    YIELD_UNTIL(NOW() + 1*SECONDS);
    PRINTF("A3.4\n");
    GOTO_LAST_YIELD_END;
}

class A4 : public Activity {
public:
    A4() : Activity("A4") { }
    void init() { activateAt(15*SECONDS); }
    void step(int64_t timeNow); 
} a4;

void A4::step([[gnu::unused]] int64_t timeNow) {
    GOTO_LAST_YIELD;
    PRINTF("A4 Initialisation\n");
    while(1) {
        PRINTF("A4.1\n");
        YIELD_UNTIL(NOW() + 500*MILLISECONDS);
        PRINTF("A4.2\n");
        YIELD_UNTIL(NOW() + 500*MILLISECONDS);
        PRINTF("A4.3\n");
        YIELD_UNTIL(NOW() + 500*MILLISECONDS);
    }
    GOTO_LAST_YIELD_END;
}

