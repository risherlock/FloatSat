#include "rodos.h"

uint32_t printfMask = 0;

static Application applic("ComBufTest-struct");

struct Position {
    float x, y, z;
};


CommBuffer<Position> buf;

class Sender : public StaticThread<> {
public:
    void run() {
        printfMask = 1;
        Position pos;
        int      cnt = 0;
        while(1) {
            cnt++;
            if (cnt > 100) {
                PRINTF("-------------------------------------------------------------");
                hwResetAndReboot();
            }
            pos.x = pos.y = pos.z = static_cast<float>(cnt);
            PRINTF("Writing %d\n", cnt);
            buf.put(pos);
            suspendCallerUntil(NOW() + 30 * MILLISECONDS);
        }
    }
};

class Receiver : public StaticThread<> {
public:
    void run() {
        printfMask = 1;
        Position mypos;
        suspendCallerUntil(NOW() + 10 * MILLISECONDS);
        while(1) {
            buf.get(mypos);
            PRINTF("Reading %d, %d, %d\n", (int)mypos.x, (int)mypos.y, (int)mypos.z);
            suspendCallerUntil(NOW() + 30 * MILLISECONDS);
        }
    }
};

/******************************/

Sender   sender;
Receiver receiver;
