//Tests for coordinate Frames

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS 100
#define RANGE           5

int coordinateFrameTests() {
    int failed = 0;
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        CoordinateFrame3D c1, c2, stdFrame;
        stdFrame = CoordinateFrame3D();         //Origin at (0,0,0) and unit vectors (1,0,0), (0,1,0) and (0,0,1)
        Vector3D v1, v2, v3, v4;
        
        v1.x = drand(RANGE);
        v1.y = drand(RANGE);
        v1.z = drand(RANGE);
        
        v2.x = drand(RANGE);
        v2.y = drand(RANGE);
        v2.z = drand(RANGE);
        
        v3.x = drand(RANGE);
        v3.y = drand(RANGE);
        v3.z = drand(RANGE);
        
        
        //Orthogonality, normalisation and check for being right-handed
        c1 = CoordinateFrame3D(v1, v2, v3);
        if (!(isAlmost0(c1.x.dot(c1.y)) && isAlmost0(c1.x.dot(c1.z)) && isAlmost0(c1.y.dot(c1.z)))) FAIL;
        if (!(isAlmost0(c1.x.getLen() - 1) && isAlmost0(c1.y.getLen() - 1) && isAlmost0(c1.z.getLen() - 1))) FAIL;
        if (!c1.z.equals(c1.x.cross(c1.y))) FAIL;
        if (!c1.y.equals(c1.z.cross(c1.x))) FAIL;
        if (!c1.x.equals(c1.y.cross(c1.z))) FAIL;
        
        //mapTo
        v1.x = drand(RANGE);
        v1.y = drand(RANGE);
        v1.z = drand(RANGE);
        
        v2.x = drand(RANGE);
        v2.y = drand(RANGE);
        v2.z = drand(RANGE);
        
        v3.x = drand(RANGE);
        v3.y = drand(RANGE);
        v3.z = drand(RANGE);
        c2 = CoordinateFrame3D(v1, v2, v3);
        Matrix4D m4d = stdFrame.mapTo(c1);//Maps from c1 to stdFrame
        
        v4.x = 0;
        v4.y = 0;
        v4.z = 0;
        if (!v4.equals(c1.origin.to4D().matVecMult(m4d.invert()).to3D())) FAIL;
        
        //Invert
        Matrix4D m4du = Matrix4D();  //Unity matrix
        
        Matrix4D m4dh = m4d.mMult(m4d.invert());
        if (!m4du.equals(m4dh)) FAIL;
        
        m4dh = m4d.invert().mMult(m4d);
        if (!m4du.equals(m4dh)) FAIL;
    }
    
    return failed;
}
