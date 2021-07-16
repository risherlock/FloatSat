/**
 * @date 2019/09/12 
 * @author Sergio Montenegro
 *
 * @brief RODOS_ERROR and asserts
*/



// #define DANGEROUS_ASSERT_ENABLED // activate to stop execution if an assert fails

#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

/**
 * I recommend to check periodically errorLog.isEmpty()
 * if it is not true some thing very wrong has occurred. 
 * I recommend to print the errorLog.printAll
 */


static Application module01("makeError");

int testReturnWithAssert() {
    PRINTF("before assert ok\n");
    RODOS_ASSERT_IFNOT_RETURN(true, 10);
    PRINTF("before assert not ok\n");
    RODOS_ASSERT_IFNOT_RETURN(false, -20);
    return 30;
}

class MakeError : public StaticThread<> {
  public:
    MakeError() : StaticThread<>("makeError") {}

    void run() {
        printfMask = 1;
        if(!errorLog.isEmpty()) {
            PRINTF("Errors prior to my execution: ");
            printErrorLog();
        }

        RODOS_ERROR("making an error");
        errorLog.add(LOCATION);

        PRINTF("assert ok\n");
        RODOS_ASSERT(true);
        PRINTF("assert wrong\n");
        RODOS_ASSERT(false);

        int x = testReturnWithAssert();
        PRINTF(" x = %d\n", x);

        PRINTF("________________________ error log:\n");
        printErrorLog();
        errorLog.clear();
        hwResetAndReboot();

    }
} makeError;

void MAIN() {
    printfMask = 1;
    PRINTF(" in MAIN... Produces an error \n");
    // Scheduler is still not running, Priority ceiling produces an error
    {
        PRIORITY_CEILER_IN_SCOPE();
        PRINTF(" error done!\n");
    }
}
