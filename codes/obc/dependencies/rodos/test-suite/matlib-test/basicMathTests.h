//Tests some basic functions of math.h, returns the number of failed tests

#define FAIL {PRINTF("FAILED at line %d in file %s\n", __LINE__, __FILE__); failed++;};

int basicMathTests() {
    int failed = 0;
    
    //Almost 0
    if (!isAlmost0(0.00000001)) FAIL;
    if (isAlmost0(1.0)) FAIL;
    
    //Grad and rad conversion
    if (!isAlmost0(0 - grad2Rad(0))) FAIL;
    if (!isAlmost0(M_PI - grad2Rad(180))) FAIL;
    if (!isAlmost0(0 - rad2Grad(0))) FAIL;
    if (!isAlmost0(90 - rad2Grad(M_PI/2))) FAIL;
    
    //Square and root
    if (!isAlmost0(9.0 - square(3))) FAIL;
    if (!isAlmost0(25.0 - square(-5))) FAIL;
    if (!isAlmost0(3.0 - sqrt(9))) FAIL;
    if (!isAlmost0(5.0 - sqrt(25))) FAIL;
    
    //frac, mod and faculty
    if (!isAlmost0(0.5 - frac(3.5))) FAIL;
    if (!isAlmost0(0.5 - frac(-3.5))) FAIL;
    
    if (!isAlmost0(2 - mod(5, 3))) FAIL;
    if (!isAlmost0(2.5 - mod(5.5, 3))) FAIL;
    if (!isAlmost0(4 - mod(-8, 6))) FAIL;
    
    if (1 != faculty(0)) FAIL;
    if (1 != faculty(1)) FAIL;
    if (120 != faculty(5)) FAIL;
    
    //Fmod2p
    if (!isAlmost0(0.5 - FMod2p(0.5))) FAIL;
    if (!isAlmost0(0.5 - FMod2p(0.5 + 4 * M_PI))) FAIL;
    if (!isAlmost0(0.5 - FMod2p(0.5 - 2 * M_PI))) FAIL;
    
    //fabs, doesn't compile, call of fabs is ambiguous, TODO
    //         if (!isAlmost0(0.5 - fabs(0.5))) FAIL;
    //         if (!isAlmost0(0.5 - fabs(-0.5))) FAIL;
    
    return failed;
} 
