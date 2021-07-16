#include "rodos.h"


class IdelCntPrinter : public StaticThread<> {
  public:
    void run() {
        float  cpuLoad;
        int64_t timeToEnd;

        getCpuLoad();
        PRINTF("\n--------------------- meassuremens every 2 seconds\n");
        PRINTF("\n----------------------- 0 cpu load:\n");
        getCpuLoad();
        suspendCallerUntil(NOW() + 2 * SECONDS);
        cpuLoad = getCpuLoad();
        PRINTF(" cpuload %f\n", static_cast<double>(cpuLoad));

        PRINTF("\n----------------------- 0.5 cpu load:\n");
        getCpuLoad();
        suspendCallerUntil(NOW() + 1 * SECONDS);
        timeToEnd = NOW() + 1 * SECONDS;
        BUSY_WAITING_UNTIL(timeToEnd);
        cpuLoad = getCpuLoad();
        PRINTF(" cpuload %f\n", static_cast<double>(cpuLoad));

        PRINTF("\n----------------------- 0.75 cpu load:\n");
        getCpuLoad();
        suspendCallerUntil(NOW() + 500 * MILLISECONDS);
        timeToEnd = NOW() + 1500 * MILLISECONDS;
        BUSY_WAITING_UNTIL(timeToEnd);
        cpuLoad = getCpuLoad();
        PRINTF(" cpuload %f\n", static_cast<double>(cpuLoad));

        PRINTF("\n----------------------- 1.0 cpu load:\n");
        getCpuLoad();
        timeToEnd = NOW() + 2 * SECONDS;
        BUSY_WAITING_UNTIL(timeToEnd);
        cpuLoad = getCpuLoad();
        PRINTF(" cpuload %f\n", static_cast<double>(cpuLoad));

        PRINTF("\n----------------------- END\n");
    }
} idelCntPrinter;
