//Tests the different possibilities to rotate a vector and multiplication of a vector3d with a matrix

#include "random.h"

#define NUMBER_OF_TESTS     100

//Range of random numbers from -RANGE to RANGE
#define RANGE               5

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};


int rotationTests() {
    int failed = 0;
    
    AngleAxis a1, a2;
    a1.phi = 0.5;
    a2.phi = a1.phi + 2 * M_PI;
    a1.u.x = 1;
    a1.u.y = 2;
    a1.u.z = 3;
    a2.u = a1.u;
    if (!a1.equals(a2)) FAIL;
    a2.phi = -1 * a1.phi;
    a2.u = -1 * a1.u;
    if (!a1.equals(a2)) FAIL;
    
    Matrix3D m;
    Vector3D a, b, c;
    a.x = -2;
    a.y = 3;
    a.z = 4;
    
    //a multiplied with identity matrix must result in a
    if (!a.equals(a.matVecMult(m))) FAIL;
    
    m.r[0][0] = 3;
    m.r[0][1] = 1;
    m.r[0][2] = 4;
    m.r[1][0] = -1;
    m.r[1][1] = 5;
    m.r[1][2] = 9;
    m.r[2][0] = 2;
    m.r[2][1] = 6;
    m.r[2][2] = -5;
    
    c = a.matVecMult(m);
    b.x = 13;
    b.y = 53;
    b.z = -6;
    if (!c.equals(b)) FAIL;
   
    //Rotations
    //Conversion between different rotation possibilities, result of rotation may not depend on used method
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        Vector3D start, aEnd, qEnd, mEnd, rEnd, h;
        start.x = drand(RANGE);
        start.y = drand(RANGE);
        start.z = drand(RANGE);
        
        h.x = drand();
        h.y = drand();
        h.z = drand();
        
        double rand_angle = abs(drand() * 2 * M_PI);
        AngleAxis random_rot = AngleAxis(rand_angle, h);
        AngleAxis convertedBack;
        
        //Conversion to other methods and back may not change the original Rotation
        convertedBack = random_rot.toRPY().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toQuaternion().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toMatrix3D().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toRPY().toMatrix3D().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toRPY().toQuaternion().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toQuaternion().toRPY().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toQuaternion().toMatrix3D().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toMatrix3D().toQuaternion().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        convertedBack = random_rot.toMatrix3D().toRPY().toAngleAxis();
        if (!random_rot.equals(convertedBack)) FAIL;
        
        Quaternion qrot = random_rot.toQuaternion();
        Matrix3D mrot = random_rot.toMatrix3D();
        RPY rpyrot = random_rot.toRPY();
        aEnd = a.aRotate(random_rot);
        qEnd = a.qRotate(qrot);
        mEnd = a.mRotate(mrot);
        rEnd = a.rpyRotate(rpyrot);
        
        if (!(aEnd.equals(qEnd) && aEnd.equals(mEnd) && aEnd.equals(rEnd))) FAIL;
        
        //Rotations around x, y and z axis
        //x
        random_rot.phi = drand(2*M_PI);
        random_rot.u.x = 1;
        random_rot.u.y = 0;
        random_rot.u.z = 0;
        mrot.rotationX(random_rot.phi);
        if (mrot != random_rot.toMatrix3D()) FAIL;
        
        //y
        random_rot.phi = drand(2*M_PI);
        random_rot.u.x = 0;
        random_rot.u.y = 1;
        random_rot.u.z = 0;
        mrot.rotationY(random_rot.phi);
        if (mrot != random_rot.toMatrix3D()) FAIL;
        
        //z
        random_rot.phi = drand(2*M_PI);
        random_rot.u.x = 0;
        random_rot.u.y = 0;
        random_rot.u.z = 1;
        mrot.rotationZ(random_rot.phi);
        if (mrot != random_rot.toMatrix3D()) FAIL;
        
    }
    
    
    
    return failed;
}
