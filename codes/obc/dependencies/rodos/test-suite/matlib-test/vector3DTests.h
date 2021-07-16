//Tests the functions of Vector3D and Polar coordinates from math.h

#include "random.h"

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS     100
#define RANGE               5

int vector3DTests() {
    int failed = 0;
    
    Vector3D a, b, c, d;        //Some vectors as helping variables, no special meanings thus no special name
    a.x = 1;
    if (a.equals(b)) FAIL;
    
    b.x = 1;
    if (!a.equals(b)) FAIL;
    
    a.x = static_cast<double>(NAN);
    b.x = 0;
    if (!a.resetIfNAN()) FAIL;
    if (!a.equals(b)) FAIL;
    if (a.resetIfNAN()) FAIL;
    
    //Set values for a and b
    a.x = 1;
    a.y = 2;
    a.z = 3;
    
    if (a.isNormalized()) FAIL;
    a = a.normalize();
    if (!a.isNormalized()) FAIL;
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        a.x = drand(RANGE);
        a.y = drand(RANGE);
        a.z = drand(RANGE);
        
        b.x = drand(RANGE);
        b.y = drand(RANGE);
        b.z = drand(RANGE);
        
        c = a.vecAdd(b);
        d.x = a.x + b.x;
        d.y = a.y + b.y;
        d.z = a.z + b.z;
        if (!c.equals(d)) FAIL;
        
        c = a.vecSub(b);
        d.x = a.x - b.x;
        d.y = a.y - b.y;
        d.z = a.z - b.z;
        if (!c.equals(d)) FAIL;
        
        double scale = drand(RANGE);
        c = a.scale(scale);
        d.x = a.x * scale;
        d.y = a.y * scale;
        d.z = a.z * scale;
        if (!c.equals(d)) FAIL;
        
        //Elementwise operations
        
        c = elementWiseProduct(a, b);
        d.x = a.x * b.x;
        d.y = a.y * b.y;
        d.z = a.z * b.z;
        if (!c.equals(d)) FAIL;
        
        c = elementWiseDivision(a, b);
        d.x = a.x / b.x;
        d.y = a.y / b.y;
        d.z = a.z / b.z;
        if (!c.equals(d)) FAIL;
        
        //Arithmetic operations
        c = a + b;
        d.x = a.x + b.x;
        d.y = a.y + b.y;
        d.z = a.z + b.z;
        if (!c.equals(d)) FAIL;
        
        c = a - b;
        d.x = a.x - b.x;
        d.y = a.y - b.y;
        d.z = a.z - b.z;
        if (!c.equals(d)) FAIL;
        
        double dot = dotProduct(a, b);
        if (!isAlmost0(dot - (a.x*b.x + a.y*b.y + a.z*b.z))) FAIL;
        
        c = crossProduct(a, b);
        d = crossProduct(b, a);
        if (!c.equals((-1) * d)) FAIL;
        d.x = a.y*b.z - a.z*b.y;
        d.y = a.z*b.x - a.x*b.z;
        d.z = a.x*b.y - a.y*b.x;
        if (!c.equals(d)) FAIL;
        
        b = scale * a;
        c.x = a.x * scale;
        c.y = a.y * scale;
        c.z = a.z * scale;
        if (!b.equals(c)) FAIL;
        
        b = a * scale;
        if (!b.equals(c)) FAIL;
        
        b = a / scale;
        c.x = a.x / scale;
        c.y = a.y / scale;
        c.z = a.z / scale;
        if (!b.equals(c)) FAIL;
        
        //Vector 4D
        Vector4D v4d = a.to4D();
        if (!(isAlmost0(v4d.x - a.x) && isAlmost0(v4d.y - a.y) && isAlmost0(v4d.z - a.z))) FAIL;
    }
    
    //Angle and orthogonality
    a.x = 1;
    a.y = 1;
    a.z = 0;
    b.x = 1;
    b.y = 0;
    b.z = 0;
    if (!isAlmost0(M_PI/4 - a.getAngle(b))) FAIL;
    if (!isAlmost0(M_PI/4 - b.getAngle(a))) FAIL;
    
    if (a.isOrthogonal(b)) FAIL;
    a.x = 0;
    if (!a.isOrthogonal(b)) FAIL;
    
    //Polarcoordinates (spherical)
    a.x = 1;
    a.y = 1;
    a.z = 1;
    Polar p1 = a.carToPolar();
    Polar p2 = Polar(sqrt(3), grad2Rad(45), acos(1/(sqrt(3))));
    if (!(isAlmost0(p1.r - p2.r) && isAlmost0(p1.phi - p2.phi) &&  isAlmost0(p1.theta - p2.theta))) FAIL;
    if (!a.equals(a.carToPolar().toCartesian())) FAIL;
    
    
    
    return failed;
}
