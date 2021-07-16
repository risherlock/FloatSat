#include "rodos.h"

static Application applic("ComBufTest-struct");

struct Position {
    float x, y, z;
};


CommBuffer<Position> buf;

class Sender : public StaticThread<> {
    void run() {
        Position pos;
        int      cnt = 0;
        while(1) {
            cnt++;
            pos.x = pos.y = pos.z = static_cast<float>(cnt);
            PRINTF("Writing %d\n", cnt);
            buf.put(pos);
            suspendCallerUntil(NOW() + 3 * SECONDS);
        }
    }
};

class Receiver : public StaticThread<> {
    void run() {
        Position mypos;
        while(1) {
            buf.get(mypos);
            PRINTF("Reading %d, %d, %d\n", (int)mypos.x, (int)mypos.y, (int)mypos.z);
            suspendCallerUntil(NOW() + 2 * SECONDS);
        }
    }
};

/******************************/

Sender   sender;
Receiver receiver;
