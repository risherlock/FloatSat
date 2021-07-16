#include "rodos.h"
#include "matlib.h"

#define DELTA_TIME 0.5
void readGyro(double& x, double& y, double& z) {
    x = 0.2 * DELTA_TIME;
    y = 0.0 * DELTA_TIME;
    z = 0.0 * DELTA_TIME;
    return; // here you shall read your device
}

void MAIN(void) {
    Quaternion lage(1, 0,0,0);
    Vector3D vx(1,0,0), v;

    double giroX, giroY, giroZ; 
    Quaternion deltaLage; 
    YPR ypr;

    for(int i = 0; i <100; i++) { // shall be: while(1) 

        ypr = lage.toYPR(); 
	v = vx.qRotate(lage);
        PRINTF("------------ Step  %d\n", i);
        PRINTF("Currten attitude quat:"); lage.print();
        PRINTF("Currten attitude v   :");    v.print();
        PRINTF("Currten attitude ypr :");  ypr.print();
        ypr = lage.toYPR(); 
        PRINTF("Currten attitude yprN:");  ypr.print();


        readGyro(giroX, giroY, giroZ);
        ypr = YPR(giroX, giroY, giroZ);
        PRINTF("gyro-ypr :\t\t ");  ypr.print();
        deltaLage = ypr.toQuaternion().normalize();
        PRINTF("gyro-quat:");  deltaLage.print();

        lage      = deltaLage * lage;  // operator* for quaternions
        
    }

    PRINTF("----------- Test END -------------------\n");

}


