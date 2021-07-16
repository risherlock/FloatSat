//These tests will test the Quaternions from matlib.h except Rotations (see rotationTests.h)

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS 100
#define RANGE           5

int quaternionTests() {
    int failed = 0;
    
    Quaternion q1, q2, q3, q4;
    double r;           //For random doubles
    
    //Reset ifNaN
    q1.q0 = static_cast<double>(NAN);
    q1.q.x = 1;
    q1.q.y = 2;
    q1.q.z = 3;
    if (!q1.resetIfNAN()) FAIL;
    if (!isAlmost0(1 - q1.q0)) FAIL;
    if (!q1.q.equals(Vector3D(0,0,0))) FAIL;
    if (q1.resetIfNAN()) FAIL;
    
    //Basic operators +, -, *, only the operators are tested as the functions qAdd, qSub, qMult and scale are called by the overloaded operators
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        q1.q0 = drand(RANGE);
        q1.q.x = drand(RANGE);
        q1.q.y = drand(RANGE);
        q1.q.z = drand(RANGE);
        
        q2.q0 = drand(RANGE);
        q2.q.x = drand(RANGE);
        q2.q.y = drand(RANGE);
        q2.q.z = drand(RANGE);
        
        //Add
        q3 = q1 + q2;
        q4.q0 = q1.q0 + q2.q0;
        q4.q = q1.q + q2.q;
        if (!q3.equals(q4)) FAIL;
        
        //Sub
        q3 = q1 - q2;
        q4.q0 = q1.q0 - q2.q0;
        q4.q = q1.q - q2.q;
        if (!q3.equals(q4)) FAIL;
        
        //Multiply with scalar
        r = drand(RANGE);
        q3 = r * q1;
        q4.q0 = q1.q0 * r;
        q4.q = q1.q * r;
        if (!q3.equals(q4)) FAIL;
        q3 = q1 * r;
        if (!q3.equals(q4)) FAIL;
        
        //Multiply with other quaternion
        q3 = q1 * q2;
        q4.q0 = q1.q0*q2.q0 - q1.q.x*q2.q.x - q1.q.y*q2.q.y - q1.q.z*q2.q.z;
        q4.q.x = q1.q0*q2.q.x + q1.q.x*q2.q0 + q1.q.y*q2.q.z - q1.q.z*q2.q.y;
        q4.q.y = q1.q0*q2.q.y + q1.q.y*q2.q0 + q1.q.z*q2.q.x - q1.q.x*q2.q.z;
        q4.q.z = q1.q0*q2.q.z + q1.q.z*q2.q0 + q1.q.x*q2.q.y - q1.q.y*q2.q.x;
        if (!q3.equals(q4)) FAIL;
        
        //Conjugation
        q3 = q1.conjugate();
        q4.q0 = q1.q0;
        q4.q = -1 * q1.q;
        if (!q3.equals(q4)) FAIL;
        q3 = -q1;
        if (!q3.equals(q4)) FAIL;
        
        //Invert
        q2 = q1.invert();
        q3 = q2 * q1;
        q4 = q1 * q2;
        if (!q3.equals(Quaternion())) FAIL;
        if (!q4.equals(Quaternion())) FAIL;
        
        //getLen
        r = sqrt(q1.q0*q1.q0 + q1.q.getLen()*q1.q.getLen());
        if (!isAlmost0(r - q1.getLen())) FAIL;
        
        //Normalise
        q2 = q1.normalize();
        if (!isAlmost0(q2.getLen() - 1)) FAIL;
        if (!q2.isNormalized()) FAIL;
        q2 = 2 * q2;        //To make sure it is NOT normalized any more
        if (q2.isNormalized()) FAIL;
        
        //Multiplication with Matrix4D
        double elements[16];
        for (int i = 0; i < 16; i++) {
            elements[i] = drand(RANGE);
        }
        Matrix4D m4d = Matrix4D(elements);
        q2 = m4d * q1;
        
        q3.q0 = m4d.r[0][0] * q1.q0 + m4d.r[0][1] * q1.q.x + m4d.r[0][2] * q1.q.y + m4d.r[0][3] * q1.q.z;
        q3.q.x = m4d.r[1][0] * q1.q0 + m4d.r[1][1] * q1.q.x + m4d.r[1][2] * q1.q.y + m4d.r[1][3] * q1.q.z;
        q3.q.y = m4d.r[2][0] * q1.q0 + m4d.r[2][1] * q1.q.x + m4d.r[2][2] * q1.q.y + m4d.r[2][3] * q1.q.z;
        q3.q.z = m4d.r[3][0] * q1.q0 + m4d.r[3][1] * q1.q.x + m4d.r[3][2] * q1.q.y + m4d.r[3][3] * q1.q.z;
        
        if (!q3.equals(q2)) FAIL;
    }
    
    return failed;
}
