/**
 * @file timepoints.cc
 * @date 2008/06/12 10:10
 * @author Lutz Dittrich
 *
 *
 * @brief test for class TimePoints
*/
#include "rodos.h"

static Application module01("TestTimePoints");

class TestThread : public StaticThread<> {

  private:
    TimePoints<30> tp;

  public:
    TestThread(const char* name = "TestThread") : StaticThread<>(name) {}

    void run() {
        int i = 0;
        tp.add("run");
        tp.add("next entry");
        tp.printAll(); tp.clear();
        while(1) {
            if((i % 30) == 0) {
                PRINTF("\n\n____________________________________\n");
                PRINTF("my own log:\n");
                tp.printAll();
                tp.clear();
                PRINTF("\nsystem errors log (Shall be empty)\n");
                errorLog.printAll();
            }
            i++;
            tp.add(" loop entry", i);
            suspendCallerUntil(NOW() + 100 * MILLISECONDS);
        }
    }
};

TestThread t;
