#include "rodos.h"
#include "activity.h"

uint32_t printfMask = 0;

class A3 : public Activity {
public:
    A3() : Activity("A") { }
    void init() { activatePeriodic(100*MILLISECONDS, 100*MILLISECONDS); }
    void step(int64_t timeNow); 
} a3;

void A3::step([[gnu::unused]] int64_t timeNow) {
    printfMask = 1;
    GOTO_LAST_YIELD;
    PRINTF("A1\n");
    YIELD_UNTIL(NOW() + 10*MILLISECONDS);
    PRINTF("A2\n");
    YIELD_UNTIL(NOW() + 10*MILLISECONDS);
    PRINTF("A3\n");
    YIELD_UNTIL(NOW() + 10*MILLISECONDS);
    PRINTF("A4\n");
    GOTO_LAST_YIELD_END;
}

class A4 : public Activity {
public:
    A4() : Activity("A4") { }
    void init() { activateAt(110*MILLISECONDS); }
    void step(int64_t timeNow); 
} a4;

void A4::step([[gnu::unused]] int64_t timeNow) {
    printfMask = 1;
    static int cnt = 0;
    cnt++;
    if (cnt > 30) {
        PRINTF("----------------------------------------------------------- end now\n");
        hwResetAndReboot();
    }
    
    GOTO_LAST_YIELD;
    PRINTF("B Initialisation\n");
    while(1) {
        PRINTF("B1\n");
        YIELD_UNTIL(NOW() + 10*MILLISECONDS);        
        PRINTF("B2\n");
        YIELD_UNTIL(NOW() + 10*MILLISECONDS);
        PRINTF("B3\n");
        YIELD_UNTIL(NOW() + 10*MILLISECONDS);
    }
    GOTO_LAST_YIELD_END;
}

