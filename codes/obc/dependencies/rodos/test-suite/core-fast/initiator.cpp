

#include "rodos.h"

uint32_t printfMask = 0;

static class MyInitiator : public Initiator {
    void init() {
        printfMask = 1;
        PRINTF(" This will be done before any thread begins\n");
        PRINTF(" Doing so you do not need to use MAIN\n");
        hwResetAndReboot();
    }
} myInitiator;
