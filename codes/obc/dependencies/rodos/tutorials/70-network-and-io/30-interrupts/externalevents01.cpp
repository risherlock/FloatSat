/**
* @file externalevents01.cpp
* @date 2008/06/11 16:07
* @author Sergio Montenegro
*
*
* @brief test of external event management
*/

#include "rodos.h"

// Announce application to the system inventory
static Application module01("externalEvents");


class SigTermHandler :  public Subscriber {
public:
    SigTermHandler() : Subscriber(interruptSigterm, "simplesub") { }
    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const void* any, size_t len) {
        xprintf("external Event at %3.9f datalen %lu\n",
            SECONDS_NOW(),
            static_cast<unsigned long>(len));
    }
} sigTermHandler;


/// Class to add a simple event handler to the system.

class MyThread : public StaticThread<> {
    void run() {
        PRINTF("In other window call killall tst\n");
        TIME_LOOP(0, 1*SECONDS) {
            PRINTF("timeloop at %3.9f, in other window please call killall tst\n", SECONDS_NOW());
        }
    }
} myThread;
