//Tests the functions of RPY except the rotations (see rotationTests.h)
#include "random.h"
#define NUMBER_OF_TESTS     100
#define RANGE               5

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};  

int RPYTests() {
    int failed = 0;

    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        RPY r1, r2, rtot;
        Vector3D start, end;
        
        start.x = drand(RANGE);
        start.y = drand(RANGE);
        start.z = drand(RANGE);
        
        r1.x = abs(drand() * 2 * M_PI);
        r1.y = abs(drand() * 2 * M_PI);
        r1.z = abs(drand() * 2 * M_PI);
        
        r2.x = abs(drand() * 2 * M_PI);
        r2.y = abs(drand() * 2 * M_PI);
        r2.z = abs(drand() * 2 * M_PI);
        
        rtot = r1.accumulateRotation(r2);
        end = start.rpyRotate(r2).rpyRotate(r1);
        
        if (!end.equals(start.rpyRotate(rtot))) FAIL;
        
    }
    
    return failed;
}
