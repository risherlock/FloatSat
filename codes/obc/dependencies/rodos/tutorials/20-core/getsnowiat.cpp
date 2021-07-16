#include "rodos.h"

static Application module01("Testgetchar");

class TestGets : public StaticThread<> {
  public:
    TestGets() : StaticThread<>("testgetchar") {}
    void run() {
        char* s;
        PRINTF("Please type string of characters\n");
        TIME_LOOP(1 * SECONDS, 1 * SECONDS) {
            if((s = getsNoWait()) != 0) { PRINTF("\n\n********* String: %s'%s'%s **********\n", SCREEN_GREEN, s, SCREEN_RESET); }
            PRINTF("Waiting 1 Seconds\n");
        }
    }
} testGets;
