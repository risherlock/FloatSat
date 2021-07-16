/**
 * @date 2019/08/12 
 * @author Sergio Montenegro
 *
 *
 * @brief test for ErrorLogs as TimePoints
*/
#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestErrorLog");

class TestErrorLog : public StaticThread<> {
  public:
    TestErrorLog() : StaticThread<>("TestErrorLog") {}

    void run() {
        if(!errorLog.isEmpty()) {
            PRINTF("Errors prior to my execution: ");
            printErrorLog();
        }

        errorLog.add("Here starts my execution");
        errorLog.add(LOCATION);

        errorLog.add("here starts my loop");
        int loopCnt = 0;
        for (int i = 0; i < 100; i++) {
            if(loopCnt > 10) {
                PRINTF("________________________ error log:\n");
                printErrorLog();
                errorLog.clear();
                loopCnt = 0;
            }
            loopCnt++;
            errorLog.add("In the loop");
            suspendCallerUntil(NOW() + 5 * MILLISECONDS);
        }
        hwResetAndReboot();
    }
} testErrorLog;

void MAIN() {
    printfMask = 1;
    PRINTF(" in MAIN... Produces an error \n");
    // Scheduler is still not running, Priority ceiling produces an error
    {
        PRIORITY_CEILER_IN_SCOPE();
        PRINTF(" error done!\n");
    }
}
