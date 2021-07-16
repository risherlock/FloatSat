//Tests for complex numbers

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

#define NUMBER_OF_TESTS 100
#define RANGE           5


int complexNumbersTests() {
    int failed = 0;
    
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        //==
        Complex c1, c2, c3, c4;
        c1.Re = drand(RANGE);
        c1.Im = drand(RANGE);
        c2.Re = c1.Re + 1;
        c2.Im = c1.Im - 1;
        if (c1 != c1) FAIL;
        if (c1 == c2) FAIL;
        
        //+, -, *, /
        c2.Re = drand(RANGE);
        c2.Im = drand(RANGE);
        c3 = c1 + c2;
        c4.Re = c1.Re + c2.Re;
        c4.Im = c1.Im + c2.Im;
        if (c3 != c4) FAIL;
        
        c3 = c1 - c2;
        c4.Re = c1.Re - c2.Re;
        c4.Im = c1.Im - c2.Im;
        if (c3 != c4) FAIL;
        
        c3 = c1 * c2;
        c4.Re = c1.Re*c2.Re - c1.Im*c2.Im;
        c4.Im = c1.Re*c2.Im + c1.Im*c2.Re;
        if (c3 != c4) FAIL;
        
        double rnd = drand(RANGE);
        c3 = rnd * c1;
        c4.Re = rnd * c1.Re;
        c4.Im = rnd * c1.Im;
        if (c3 != c4) FAIL;
        
        c3 = c1 * rnd;
        if (c3 != c4) FAIL;
        
        c3 = c1 / rnd;
        c4.Re = c1.Re / rnd;
        c4.Im = c1.Im / rnd;
        if (c3 != c4) FAIL;
        
        //cPow
        c3 = c1.cPow(1);
        if (c1 != c3) FAIL;
        
        c3 = c1.cPow(0);
        c4.Re = 1;
        c4.Im = 0;
        if (c3 != c4) FAIL;
        
        int expInt = static_cast<int>(drandPositive(10));
        c3 = c1.cPow(expInt);
        for (int j = 0; j < expInt; j++) {
            c4 = c1 * c4;
        }
        if (c3 != c4) FAIL;
        
        //cExp
        c3 = c1.cExp();
        c4.Re = exp(c1.Re) * cos(c1.Im);
        c4.Im = exp(c1.Im) * sin(c1.Im);
    }
    
    
    return failed;
}
