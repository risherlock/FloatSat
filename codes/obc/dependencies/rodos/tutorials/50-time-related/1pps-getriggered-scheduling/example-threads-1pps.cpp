#include "rodos.h"
#include "activity1pps.h"


/***************************************/
class T1 : public StaticThread<> {
  public:
    void run() {
        while(1) {
            suspendCallerUntil();
            PRINTF("T1 at  %6.3f\n", SECONDS_NOW());
        }
    }
} t1;
Activity1PPS at1(1, 100*MILLISECONDS, &t1, "at1");

/***************************************/
class T2 : public StaticThread<> {
  public:
    void run() {
        while(1) {
            suspendCallerUntil();
            PRINTF("T2 at  %6.3f\n", SECONDS_NOW());
        }
    }
} t2;
Activity1PPS at2(2, 0*MILLISECONDS, &t2, "at2");

