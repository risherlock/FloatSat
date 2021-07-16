#include "rodos.h"

#include "fifo.h"

uint32_t printfMask = 0;
class FiFoTester : public StaticThread<> {

  public:
    void run() {
        printfMask = 1;
        Fifo<int32_t, 3> fifo; //A maximum of 3-1=2 ints in the Fifo
        int32_t          tmp = 0;
        bool             ok  = false;

        ok = fifo.put(1);
        PRINTF("1. write success %d\n", ok);

        ok = fifo.get(tmp);
        PRINTF("read success %d, data  %d\n", ok, static_cast<int>(tmp));

        ok = fifo.get(tmp);
        PRINTF("read success %d (expected 0)\n", ok);

        ok = fifo.isEmpty();
        PRINTF("is empty  %d\n", ok);

        ok = fifo.put(2);
        PRINTF("2. write success %d\n", ok);
        ok = fifo.put(3);
        PRINTF("3. write success %d\n", ok);
        ok = fifo.put(4);
        PRINTF("4. write success %d (expected 0)\n", ok);

        for(int i = 0; i < 5; i++) {
            ok = fifo.get(tmp);
            PRINTF("read success %d, data %d\n", ok, static_cast<int>(tmp));
        }

        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();
    }
} fifoTester;
