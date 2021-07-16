//Tests the functions of rotor

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;}; 

#define NUMBER_OF_TESTS 100

int rotorTests() {
    int failed = 0;
    
    Rotor rot;
    Vector3D start, end;
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        start.x = drand(RANGE);
        start.y = drand(RANGE);
        start.z = drand(RANGE);
        
        AngleAxis aRot(abs(drand() * 2 * M_PI), Vector3D(drand(RANGE), drand(RANGE), drand(RANGE)));
        end = start.aRotate(aRot);
        
        rot = Rotor(start, end);
        if (!end.equals(start.qRotate(Quaternion(rot)))) FAIL;
    }
    
    //isNoRotation
    rot.axis.x = 1;
    rot.axis.y = 2;
    rot.axis.z = 3;
    rot.cosAngle = 1;
    if (!rot.isNoRotation()) FAIL;
    
    rot.cosAngle = 0.5;
    if (rot.isNoRotation()) FAIL;
    
    rot.cosAngle = 1.5;
    if (!rot.isNoRotation()) FAIL;
    rot.cosAngle = -1.5;
    if (!rot.isNoRotation()) FAIL;
    
    rot.axis.x = 0;
    rot.axis.y = 0;
    rot.axis.z = 0;
    rot.cosAngle = 0.5;
    if (!rot.isNoRotation()) FAIL;
    
    //ifNaN
    rot.axis.x = 1;
    rot.axis.y = 2;
    rot.axis.z = 3;
    rot.cosAngle = 1;
    if (rot.resetIfNAN()) FAIL;
    
    rot.axis.x = static_cast<double>(NAN);
    if (!rot.resetIfNAN()) FAIL;
    if (!rot.axis.equals(Vector3D(1, 1, 1))) FAIL;
    
    rot.axis.y = static_cast<double>(NAN);
    if (!rot.resetIfNAN()) FAIL;
    rot.axis.z = static_cast<double>(NAN);
    if (!rot.resetIfNAN()) FAIL;
    rot.cosAngle = static_cast<double>(NAN);
    if (!rot.resetIfNAN()) FAIL;
    
    return failed;
}
