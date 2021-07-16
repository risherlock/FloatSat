#include "rodos.h"

SyncFifo<int, 10> fifo;

class Sender : public StaticThread<> {
    void run() {
        int cnt = 0;
        suspendCallerUntil(NOW() + 5 * SECONDS);
        while(1) {
            cnt++;
            bool ok = fifo.syncPut(cnt);
            if(ok) {
                xprintf("sent %d\n", cnt);
            } else {
                xprintf("Fifo full\n");
            }
            if((cnt % 20) == 0) {
                xprintf("Sender Wainting 3 seconds\n");
                suspendCallerUntil(NOW() + 3 * SECONDS);
            }
        }
    }
};

class Receiver : public StaticThread<> {
    void run() {
        int cntIn = 0;
        int cnt   = 0;
        while(1) {
            cnt++;
            bool ok = fifo.syncGet(cntIn);
            if(ok) {
                xprintf("read %d\n", cntIn);
            }
            if((cnt % 50) == 0) {
                xprintf("Reciver wait 15 seconds\n");
                suspendCallerUntil(NOW() + 15 * SECONDS);
            }
        }
    }
};

/******************************/

Sender   sender;
Receiver receiver;
