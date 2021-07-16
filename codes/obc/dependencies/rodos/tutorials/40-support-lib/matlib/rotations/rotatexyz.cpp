#include "rodos.h"
#include "matlib.h"


void MAIN(void) {
    Quaternion lage(1, 0,0,0);
    Vector3D vx(1,0,0), v;
    Quaternion deltaLage; 
    YPR ypr;
    AngleAxis rotor(grad2Rad(120), 1, 1, 1);

    v = vx;

    for(int i = 0; i <10; i++) { // shall be: while(1) 
        PRINTF("Current attitude v   :");    v.print();
        v = v.aRotate(rotor);
    }

    PRINTF("----------- Test END -------------------\n");

}


