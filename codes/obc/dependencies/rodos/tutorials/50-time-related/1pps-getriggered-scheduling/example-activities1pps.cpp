#include "rodos.h"
#include "activity1pps.h"

class A1 : public Activity1PPS {
  public:
    A1() : Activity1PPS(2, 100 * MILLISECONDS, 0, "A1") {}
    void step(int64_t timeNow) { PRINTF("A1 at %6.3f\n", (double)timeNow / (double)SECONDS); }
} a1;


class A2 : public Activity1PPS {
  public:
    A2() : Activity1PPS(4, 50 * MILLISECONDS, 0, "A2") {}
    void step(int64_t timeNow) { PRINTF("A2 at %6.3f\n", (double)timeNow / (double)SECONDS); }
} a2;

class A3 : public Activity1PPS {
  public:
    A3() : Activity1PPS(1, 500 * MILLISECONDS, 0, "A2") {}
    void step(int64_t timeNow) { PRINTF("A3 at %6.3f\n", (double)timeNow / (double)SECONDS); }
} a3;
