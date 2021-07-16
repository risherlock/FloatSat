

#include "rodos.h"

static class MyInitiator : public Initiator {
    void init() {
        xprintf(" This will be done before any thread begins\n");
        xprintf(" Doing so you do not need to use MAIN\n");
    }
} myInitiator;
