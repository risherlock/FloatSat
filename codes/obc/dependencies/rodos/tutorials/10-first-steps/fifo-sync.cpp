#include "rodos.h"

SyncFifo<int, 10> fifo;

class Sender : public StaticThread<> {
    void run() {
        int cnt = 0;
        AT(NOW() + 5 * SECONDS);
        while(1) {
            cnt++;
            bool ok = fifo.syncPut(cnt);
            if(ok) {
                PRINTF("sent %d\n", cnt);
            } else {
                PRINTF("Fifo full\n");
            }
            if((cnt % 20) == 0) {
                PRINTF("Sender Wainting 3 seconds\n");
                AT(NOW() + 3 * SECONDS);
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
                PRINTF("read %d\n", cntIn);
            }
            if((cnt % 50) == 0) {
                PRINTF("Reciver wait 15 seconds\n");
                AT(NOW() + 15 * SECONDS);
            }
        }
    }
};

Sender   sender;
Receiver receiver;
