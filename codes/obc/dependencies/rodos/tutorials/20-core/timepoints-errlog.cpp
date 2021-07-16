/**
 * @date 2019/08/12 
 * @author Sergio Montenegro
 *
 *
 * @brief test for ErrorLogs as TimePoints
*/
#include "rodos.h"

static Application module01("TestErrorLog");

class TestErrorLog : public StaticThread<> {
  public:
    TestErrorLog() : StaticThread<>("TestErrorLog") {}

    void run() {
        if(!errorLog.isEmpty()) {
            PRINTF("Errors prior to my execution: ");
            errorLog.printAll();
        }

        errorLog.add("here starts my execution");
        errorLog.add(LOCATION);

        errorLog.add("here starts my loop");
        int loopCnt = 0;
        while(1) {
            if(loopCnt > 10) {
                PRINTF("________________________ error log:\n");
                errorLog.printAll();
                errorLog.clear();
                loopCnt = 0;
            }
            loopCnt++;
            errorLog.add("In the loop");
            suspendCallerUntil(NOW() + 500 * MILLISECONDS);
        }
    }
} testErrorLog;

void MAIN() {
    PRINTF(" in MAIN... Produces an error \n");
    // Scheduler is still not running, Priority ceiling produces an error
    {
        PRIORITY_CEILER_IN_SCOPE();
        PRINTF(" error done!\n");
    }
}
