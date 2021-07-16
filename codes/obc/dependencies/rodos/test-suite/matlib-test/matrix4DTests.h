//Tests the functions of Matrix4D from matlib.h

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS 100
#define RANGE           5


int matrix4DTests() {
    int failed = 0;
    
    Matrix4D m1, m2, m3, m4;
    double aux;
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        //Equals
        double rnd[16];
        for (int j = 0; j < 16; j++) {
            rnd[j] = drand(RANGE);
        }
        m1 = Matrix4D(rnd);
        rnd[0] += 1;
        m2 = Matrix4D(rnd);         //Can't be equal to m1
        if (!m1.equals(m1)) FAIL;
        if (m1.equals(m2)) FAIL;
        
        //Scale
        for (int j = 0; j < 16; j++) {
            rnd[j] = drand(RANGE);
        }
        m1 = Matrix4D(rnd);
        
        aux = drand(RANGE);
        m3 = m1.scale(aux);
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                m4.r[j][k] = aux * m1.r[j][k];
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        //Multiplication
        for (int j = 0; j < 16; j++) {
            rnd[j] = drand(RANGE);
        }
        m2 = Matrix4D(rnd);
        
        m3 = m1.mMult(m2);
        
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                m4.r[j][k] = 0;
            }
        }
        
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                for (int l = 0; l < 4; l++) {
                    m4.r[j][k] += m1.r[j][l] * m2.r[l][k];
                }
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        //Invert
        Matrix3D m3d1, m3d2, m3d3;
        m3d1.rotationX(drand(2*M_PI));
        m3d2.rotationY(drand(2*M_PI));
        m3d3.rotationZ(drand(2*M_PI));
        
        m1 = Matrix4D(m3d1*m3d2*m3d3, Vector3D(drand(RANGE), drand(RANGE), drand(RANGE)));
        
        m2 = m1.invert();
        m4 = Matrix4D();
        m3 = m1.mMult(m2);
        if (!m3.equals(m4)) FAIL;
    
        m3 = m2.mMult(m1);
        if (!m3.equals(m4)) FAIL;
    }
    
    
    return failed;    
}
 
