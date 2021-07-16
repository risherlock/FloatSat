//Tests the functions of Vector6D

#include "random.h"

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS     100
#define RANGE               5

int vector6DTests() {
    int failed = 0;
    
    Vector6D v1, v2, v3, v4;
    
    double rnd[6];
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        for (int j = 0; j < 6; j++) {
            rnd[j] = drand(RANGE);
        }
        v1 = Vector6D(rnd);
        
        for (int j = 0; j < 6; j++) {
            rnd[j] = drand(RANGE);
        }
        v2 = Vector6D(rnd);
        
        // +
        v3 = v1 + v2;
        for (int j = 0; j < 6; j++) {
            v4.v[j] = v1.v[j] + v2.v[j];
        }
        if (v3 != v4) FAIL;
        
        // -
        v3 = v1 - v2;
        for (int j = 0; j < 6; j++) {
            v4.v[j] = v1.v[j] - v2.v[j];
        }
        if (v3 != v4) FAIL;
        
        // *
        double r = drand(RANGE);
        v3 = r * v1;
        for (int j = 0; j < 6; j++) {
            v4.v[j] = v1.v[j] * r;
        }
        if (v3 != v4) FAIL;
        
        v3 = v1 * r;
        if (v3 != v4) FAIL;
        
        // /
         v3 = v1 / r;
        for (int j = 0; j < 6; j++) {
            v4.v[j] = v1.v[j] / r;
        }
        if (v3 != v4) FAIL;
        
        //getLen
        r = 0;
        for (int j = 0; j < 6; j++) {
            r += v1.v[j] * v1.v[j];
        }
        r = sqrt(r);
        if (!isAlmost0(r - v1.getLen())) FAIL;
        
        //dotProduct
        r = 0;
        for (int j = 0; j < 6; j++) {
            r += v1.v[j] * v2.v[j];
        }
        if (!isAlmost0(r - dotProduct(v1, v2))) FAIL;
        if (!isAlmost0(r - dotProduct(v2, v1))) FAIL;
        
        //matVecMult
        double r2[36];
        for (int j = 0; j < 36; j++) {
            r2[j] = drand(RANGE);
        }
        Matrix6D m6d = Matrix6D(r2);
        
        v3 = v1.matVecMult(m6d);
        v4 = Vector6D();
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                v4.v[j] += m6d.r[j][k] * v1.v[k];
            }
        }
        if (v3 != v4) FAIL;
    }
    
    return failed;
}
 
