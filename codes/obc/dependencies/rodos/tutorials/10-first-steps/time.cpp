#include "rodos.h"

static Application nameNotImportantTM("TestTimeAT");

static class TestTime : public StaticThread<> {

  public:
    TestTime() : StaticThread<>("waitAT") {}

    void run() {

        PRINTF("waiting until 3rd second after start\n");
        AT(3 * SECONDS);
        PRINTF("after 3rd second\n");

        PRINTF("waiting until 1 second has pased\n");
        AT(NOW() + 1 * SECONDS);
        PRINTF("1 second has pased\n");

        PRINTF("print every 2 seconds, start at 5 seconds\n");
        TIME_LOOP(5 * SECONDS, 2 * SECONDS) {
            PRINTF("current time: %3.9f\n", SECONDS_NOW());
        }
    }

    void init() { PRINTF("Waiting time"); }

} testTime;
