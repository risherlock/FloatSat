

#include "rodos.h"

uint32_t printfMask = 0;

class MyInitiator : public Initiator {
  public:
    void init() {
        printfMask = 1;
        PRINTF(" This will be done before any thread begins\n");
        PRINTF(" Doing so you do not need to use MAIN\n");
        printfMask = 0;
    }
} myInitiator;

class MyInitiatorApplic : public Application {
  public:
    MyInitiatorApplic() : Application("initatorApplic", 2000) {}
    void init() {
        printfMask = 1;
        PRINTF(" This is the initator from applications\n");
        hwResetAndReboot();
    }
} mayInitiatorApplic;
