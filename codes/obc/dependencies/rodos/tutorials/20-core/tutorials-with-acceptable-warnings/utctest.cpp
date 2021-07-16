#include "rodos.h"


/*********************************************************************/

void MAIN() {
    int64_t timeNow;

    timeNow = TimeModel::calendar2LocalTime(2008, 11, 12, 17, 35, 34.21);
    xprintf("Time now (now == creation of this prog) in localTime (nanoseconds)  = %lld\n", static_cast<long long>(timeNow));
    sysTime.setUTC(timeNow);
}


static Application module01("utcprinter");

class PrintTime : public StaticThread<> {
  public:
    PrintTime() : StaticThread<>("printtime") {}
    void init() { PRINTF("Waiting 2 seconds"); }
    void run();
};

void PrintTime::run() {
    int32_t year;
    int32_t month;
    int32_t day;
    int32_t hour;
    int32_t min;
    double  sec;

    uint32_t ccsdsSeconds;
    uint32_t ccsdsFraction;

    int64_t utcNow;
    TIME_LOOP(0, 2 * SECONDS) {
        utcNow = sysTime.getUTC();
        PRINTF("UTC in localTime (nanoseconds)  = %lld  ", static_cast<long long>(utcNow));
        TimeModel::localTime2Calendar(utcNow, year, month, day, hour, min, sec);
        PRINTF("  -> %d.%d.%d  %d:%d:  %6.3f\n",
               (int)day, (int)month, (int)year, (int)hour, (int)min, sec);

        TimeModel::splitTimeECSS(NOW(), ccsdsSeconds, ccsdsFraction);
        PRINTF("CCSDS Time = (%d 0x%08x).(%d 0x%08x)\n", (int)ccsdsSeconds, (int)ccsdsSeconds, (int)ccsdsFraction, (int)ccsdsFraction);
    }
}

static PrintTime printTime;
