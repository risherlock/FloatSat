#include "rodos.h"

static Application nameNotImportantCB("ComBufTest");

CommBuffer<int> buf;

class Sender : public StaticThread<> {
    void run() {
        int cnt = 0;
        while(1) {
            cnt++;
            PRINTF("Writing %d\n", cnt);
            buf.put(cnt);
            AT(NOW() + 3 * SECONDS);
        }
    }
};

class Receiver : public StaticThread<> {
    void run() {
        int cnt;
        while(1) {
            buf.get(cnt);
            PRINTF("Reading %d\n", cnt);
            AT(NOW() + 2 * SECONDS);
        }
    }
};

Sender   sender;
Receiver receiver;
