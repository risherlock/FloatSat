//Tests the functions of Matrix3D from matlib.h, except rotations

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS 100
#define RANGE           5


int matrix3DTests() {
    int failed = 0;
    
    Matrix3D m1, m2, m3, m4, ident;
    ident = Matrix3D();
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        //getAngle and getVec
        AngleAxis aa, aa2;
        aa.phi = abs(drand(2 * M_PI));
        aa.u.x = drand(RANGE);
        aa.u.y = drand(RANGE);
        aa.u.z = drand(RANGE);
        aa.u = aa.u.normalize();
        
        m1 = Matrix3D(aa);
        aa2.phi = m1.getAngle();
        aa2.u = m1.getVec();
        
        if (!aa.equals(aa2)) FAIL;
        if (!aa.equals(aa2)) {
            PRINTF("Angles: %f      %f\n", m1.getAngle(), aa.phi);
            m1.getVec().print();
            aa.u.print();
        }
        
        //Invertation
        double rnd[9];
        for (int j = 0; j < 9; j++) {
            rnd[j] = drand(RANGE);
        }
        m1 = Matrix3D(rnd);
        if (m1.determinant() != 0) {
            if (!ident.equals(m1.invert() * m1)) FAIL;
            if (!ident.equals(m1 * m1.invert())) FAIL;
        }
        
        //+ and -
        for (int j = 0; j < 9; j++) {
            rnd[j] = drand(RANGE);
        }
        m2 = Matrix3D(rnd);
        m3 = m1 + m2;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                m4.r[j][k] = m1.r[j][k] + m2.r[j][k];
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        m3 = m1 - m2;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                m4.r[j][k] = m1.r[j][k] - m2.r[j][k];
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        //Determinate
        double aux = 0;
        aux += m1.r[0][0] * m1.r[1][1] * m1.r[2][2];
        aux += m1.r[0][1] * m1.r[1][2] * m1.r[2][0];
        aux += m1.r[0][2] * m1.r[1][0] * m1.r[2][1];
        
        aux -= m1.r[2][0] * m1.r[1][1] * m1.r[0][2];
        aux -= m1.r[2][1] * m1.r[1][2] * m1.r[0][0];
        aux -= m1.r[2][2] * m1.r[1][0] * m1.r[0][1];
        
        if (!isAlmost0(aux - m1.determinant())) FAIL;
        
        //Transpose
        m3 = m1.transpose();
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (!isAlmost0(m1.r[j][k] - m3.r[k][j])) FAIL;
            }
        }
        
        //isOrthogonal
        Vector3D v1, v2, v3;
        v1.x = drand(RANGE);
        v1.y = drand(RANGE);
        v1.z = drand(RANGE);
        
        v2.x = drand(RANGE);
        v2.y = drand(RANGE);
        v3.z = drand(RANGE);
        
        v3 = v1.cross(v2);
        v2 = v3.cross(v1);
        v1 = v1.normalize();
        v2 = v2.normalize();
        v3 = v3.normalize();
        m1 = Matrix3D(v1, v2, v3);
        
        if (!m1.isOrthogonal()) FAIL;
        //NOT orthogonal any more
        m1.r[0][0] += 10;
        if(m1.isOrthogonal()) FAIL;
        
        //Scale
        aux = drand(RANGE);
        m3 = m2.scale(aux);
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                m4.r[j][k] = m2.r[j][k] * aux;
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        //Multiply and divide
        for (int j = 0; j < 9; j++) {
            rnd[j] = drand(RANGE);
        }
        m1 = Matrix3D(rnd);
        for (int j = 0; j < 9; j++) {
            rnd[j] = drand(RANGE);
        }
        m2 = Matrix3D(rnd);
        
        m3 = m1 * m2;
        
        m4 = m4 - m4;       //Set all elements to 0
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    m4.r[j][k] += m1.r[j][l] * m2.r[l][k];
                }
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        m3 = m1.mDivide(m2);
        m4 = m1 * m2.invert();
        if (!m3.equals(m4)) FAIL;
        
        //cofac
        m3 = m1.cofac();
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                m4.r[j][k] = m1.r[(j+1)%3][(k+1)%3] * m1.r[(j+2)%3][(k+2)%3] - m1.r[(j+1)%3][(k+2)%3] * m1.r[(j+2)%3][(k+1)%3];
            }
        }
        if (!m3.equals(m4)) FAIL;
        
        //adjoint
        m3 = m1.adjoint();
        m4 = m4.transpose();
        if (!m3.equals(m4)) FAIL;
        
        //Trace
        aux = 0;
        for (int j = 0; j < 3; j++) {
            aux += m1.r[j][j];
        }
        if (!isAlmost0(aux - m1.trace())) FAIL;
    }
    
    return failed;    
}
