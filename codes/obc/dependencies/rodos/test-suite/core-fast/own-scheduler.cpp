#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestTime");

class TestTime : public StaticThread<> {
  private:
    int id;
  public:
  TestTime(int id) : StaticThread<>("waitfor"), id(id) {}
    void run() {
        printfMask = 1;
        int cnt = 0;
        while(1) {
            cnt++;
            suspendCallerUntil();
            PRINTF(" %x, After 0.02 Seconds  : %3.9f %d\n", id, CNT_SECONDS_NOW(), cnt);
        }
    }
};


TestTime t[] = {1, 2, 3, 4};


class MySchedluler : public StaticThread<> {
  public:
    MySchedluler() : StaticThread<>("mySchedluler", 10) {}
    void run() {
        printfMask = 1;
        int cnt = 0;
        TIME_LOOP(100 * MILLISECONDS, 20 * MILLISECONDS) {
            cnt++;
            if (cnt > 25) {
                hwResetAndReboot();
            }
            PRINTF("-----------------------------------\n");
            for(int i = 0; i < 4; i++) {
                t[i].resume();
                yield();
            }
        }
    }
} mySchelduer;
