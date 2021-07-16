
/**
 *
 * @date 2019/08/03
 * @author Sergio Montenegro, Uni Wuerzburg
 *
 * @brief demonstrate how ERROR
 *
 */

#include "rodos.h"


enum class AnomalyId : uint32_t {
    ANOMALY_1 = 10001, // for application errors begin at 10000
    ANOMALY_2 = 10002,
    ANOMALY_3 = 10003
};


class TestError : public StaticThread<> {
    void run() {
        PRINTF("\n\n________________________________- Error log (shall be empty)\n");
        if(!errorLog.isEmpty()) errorLog.printAll();


        printErrorReports = false; // Silent!
        RODOS_ERROR("First error");
        RODOS_ERROR("Second error");

        PRINTF("\n\n________________________________- Error log (first and second)\n");
        if(!errorLog.isEmpty()) errorLog.printAll();

        PRINTF("\n\n________________________________ now verbose error reports\n");
        printErrorReports = true; // verbose
        RODOS_ERROR("Error nr 3");
        RODOS_ERROR("Error nr 4");

        PRINTF("\n\n________________________________- Error log (first and second)\n");
        if(!errorLog.isEmpty()) errorLog.printAll();

    }
} testError;


