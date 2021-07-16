//Tests the functions of Vector4D

#include "random.h"

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};  

#define NUMBER_OF_TESTS     100
#define RANGE               5

int vector4DTests() {
    int failed = 0;
    
    double elements[16];    
    Matrix4D m4d;
    Vector4D v4d, res, res2;
    
    //Identity matrix
    for (int i = 0; i < 16; i++) {
        if ((i % 5) == 0) {
            elements[i] = 1;
        } else {
            elements[i] = 0;
        }
    }
    m4d = Matrix4D(elements);
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        v4d.x = drand(RANGE);
        v4d.y = drand(RANGE);
        v4d.z = drand(RANGE);
        
        res = v4d.matVecMult(m4d);
        
        if (!(isAlmost0(v4d.x - res.x) && isAlmost0(v4d.y - res.y) && isAlmost0(v4d.z - res.z))) FAIL;
    }
    
    //Arbitrary matrix
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        v4d.x = drand(RANGE);
        v4d.y = drand(RANGE);
        v4d.z = drand(RANGE);
        
        for (int i = 0; i < 12; i++) {
            elements[i] = drand(RANGE);
        }
        elements[12] = 0;
        elements[13] = 0;
        elements[14] = 0;
        elements[15] = 1;
        m4d = Matrix4D(elements);
        
        res = v4d.matVecMult(m4d);
        
        res2.x = elements[0]*v4d.x + elements[1]*v4d.y + elements[2]*v4d.z + elements[3]*1;
        res2.y = elements[4]*v4d.x + elements[5]*v4d.y + elements[6]*v4d.z + elements[7]*1;
        res2.z = elements[8]*v4d.x + elements[9]*v4d.y + elements[10]*v4d.z + elements[11]*1;
        
        if (!(isAlmost0(res2.x - res.x) && isAlmost0(res2.y - res.y) && isAlmost0(res2.z - res.z))) FAIL;
    }
    
    return failed;
}
