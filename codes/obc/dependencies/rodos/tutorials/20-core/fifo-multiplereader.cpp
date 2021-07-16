#include "rodos.h"

static Application applic("FifoTest");

MultipleReaderFifo<int, 100, 5> fifo;

class Sender : public StaticThread<> {
    void run() {
        int cnt = 0;
        xprintf("sender\n");
        while(1) {
            fifo.put(cnt++);
            if((cnt % 15) == 0) {
                PRINTF("Wainting 3 seconds\n");
                suspendCallerUntil(NOW() + 3 * SECONDS);
            }
        }
    }
};


class Receiver : public StaticThread<> {
public:
    uint32_t myId;
    Receiver(uint32_t _myId) : myId(_myId) { }

    void run() {
        int cnt;
        suspendCallerUntil(NOW() + myId * SECONDS); 

        while(1) {
            bool ok = fifo.get(cnt, myId);
            if(ok) {
                PRINTF("(%d)reading %05d\n", myId, cnt);
            } else {
                PRINTF("(%d)Nothing! -> wait\n", myId);
                suspendCallerUntil(NOW() + 1 * SECONDS);
            }
        }
    }
};

/******************************/

Sender   sender;
Receiver receiver[5] = {0,1,2,3,4};
