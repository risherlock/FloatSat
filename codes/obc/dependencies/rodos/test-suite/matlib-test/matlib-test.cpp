#include "rodos.h"
#include "matlib.h"

//Files with tests
#include "basicMathTests.h"
#include "vector3DTests.h"
#include "rotationTests.h"
#include "rotorTests.h"
#include "RPYTests.h"
#include "vector4DTests.h"
#include "quaternionTests.h"
#include "matrix3DTests.h"
#include "matrix4DTests.h"
#include "complexTests.h"
#include "coordinateFrameTests.h"
#include "vector6DTests.h"
#include "matrix6DTests.h"


//All numbers in this tests were chosen by the programmer. They do NOT have any special meaning and can be changed as long as all calculation remain correct.

uint32_t printfMask = 0;

class MatlibTests : public StaticThread<> {
    void run() {
        int failed = 0;
        // uint64_t start = NOW();
        printfMask = 1;
    
        failed += basicMathTests();
        failed += vector3DTests();
        failed += rotationTests();
        failed += rotorTests();
        failed += RPYTests();
        failed += vector4DTests();
        failed += quaternionTests();
        failed += matrix3DTests();
        failed += matrix4DTests();
        failed += complexNumbersTests();
        failed += coordinateFrameTests();
        failed += vector6DTests();
        failed += matrix6DTests();
        
        // PRINTF("Total time: %f ms\n", (NOW() - start) / 1000000.0);
        
        //End of tests-----------------------------------------------
        if(failed == 0) PRINTF ("All matlib tests pass!\n");
        else            PRINTF("%d Test(s) failed\n", failed);
        hwResetAndReboot();
    }
} mlt;
