#include "rodos.h"
#include "gateway.h"

Topic<long>   counter5(1002, "counter5");
Topic<long>   counter6(1003, "counter6");


/******************************/

static UDPInOut udp(-50000);
static LinkinterfaceUDP linkinterfaceUDP(&udp);
static Gateway gateway1(&linkinterfaceUDP, true);

/******************************/

class MyPublisher16 : public Thread {
public:
    MyPublisher16() : Thread("sender16") { }
    void run () {
        long cnt = 10000;
        TIME_LOOP(0, 1600*MILLISECONDS) {
            PRINTF("sending %ld\n", --cnt);
            counter6.publish(cnt);
        }
    }
} myPublisher16;


struct Receiver : public Subscriber {
    Receiver() : Subscriber(counter5, "justprint11") { }
    long put(const long topicId, const long len, const void* msg, const NetMsgInfo& netMsgInfo) {
        PRINTF(" got %ld\n",  *(long*)msg);
        return true;
    }
} justPrint11;

