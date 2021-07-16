
/**
 *
 * @date 2019/08/03
 * @author Sergio Montenegro, Uni Wuerzburg
 *
 * @brief demonstrate how to use the Result class to return values or error codes
 *
 */

#include "rodos.h"
#include "rodos-result.h"

using namespace RODOS;

enum class AnomalyId : uint32_t {
    ANOMALY_1 = 10001, // for application errors begin at 10000
    ANOMALY_2 = 10002,
    ANOMALY_3 = 10003
};

struct Vect3D {
    double x, y, z;
};

//__________________________________________________________ example for functions returning "Result"

Result<int32_t> f1() { return 123456; }             // ok, no error
Result<int32_t> f2() { return ErrorCode::UNKNOWN; } // no value, only error code
Result<int32_t> f3() { return (ErrorCode)AnomalyId::ANOMALY_1; }

Result<Vect3D> f5(int num) {
    Vect3D v = { 0, 0, 1 };
    for(int i = 0; i < 10; i++) {
        v.x += num;
        v.y -= num;
        v.z *= num;
    }
    if(v.x < 20) return { ErrorCode::NUMERICAL, v }; // error and despite of it it returns a value which could be used
    return v;                                        // ok no error
}

Result<double> f6(int num) {
    if(num < 0) return (ErrorCode)AnomalyId::ANOMALY_2;
    return 3.1315926;
}


//__________________________________________________________ example for calling functions which return "Result"

class TestResults : public StaticThread<> {
    void run() {
        printErrorReports = false; // aktivate/deactivate this line and test again
        Result<int32_t> x;

        //_______________________________________________
        if((x = f1()).isOk()) {
            PRINTF(" f1()  : val = %d\n", (int)x.val);
        } else {
            PRINTF(" f1()  : error %d\n", (int)x.err);
        }

        //_______________________________________________
        if((x = f2()).isOk()) {
            PRINTF(" f2()  : val = %d\n", (int)x.val);
        } else {
            PRINTF(" f2()  : error %d\n", (int)x.err);
        }

        //_______________________________________________
        Result<double> r1 = f6(-20);
        if(r1.isOk()) {
            PRINTF(" f6(-20): is  ok : %f\n", r1.val);
        } else {
            PRINTF(" f6(-20): is  not ok err = %d\n", (int)r1.err);
        }


        /** Ignoring error codes, compatible with old code : **/
        /** I do not recommend to do it in this way          **/
        //_____________________________________________________
        int32_t num = f1();
        PRINTF(" f1()  : I do not care about errors, result = %d\n", static_cast<int>(num));
        num = f3();
        PRINTF(" f3()  : I do not care about errors, (wrong) result = %d\n", static_cast<int>(num));

        //_______________________________________________
        Vect3D  vv;
        int32_t f5Err;

        vv    = f5(1);
        f5Err = f5(1).getErr();
        PRINTF(" f5(1) : I do not care about errors, vect3d=  (%f %f %f), err = %d\n", vv.x, vv.y, vv.z, static_cast<int>(f5Err));

        //_______________________________________________
        PRINTF("___ as it shall be used\n");
        Result<Vect3D> good = f5(10);
        PRINTF(" f5(10): I do not care about errors, vect3d=  (%f %f %f), err = %d\n", good.val.x, good.val.y, good.val.z, static_cast<int>(good.err));

        Result<Vect3D> r = f5(1);
        PRINTF(" f5(1) : err = %d vect3d = (%f %f %f)\n", (int)r.err, r.val.x, r.val.y, r.val.z);

        r  = f5(3);
        vv = r;
        PRINTF(" f5(3) : just assign      = (%f %f %f)\n", vv.x, vv.y, vv.z);

        // --- demonstrate what  if I use a result which was wrong
        double valKo = f6(-3);
        PRINTF(" a value which shall be undefined: %f\n", valKo);


        PRINTF("\n\n________________________________- Error log \n");
        if(!errorLog.isEmpty()) errorLog.printAll();
    }
} testResult;


