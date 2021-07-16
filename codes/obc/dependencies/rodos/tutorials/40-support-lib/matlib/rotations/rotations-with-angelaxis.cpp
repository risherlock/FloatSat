#include "rodos.h"
#include "matlib.h"

void MAIN() {

    Vector3D vx(1 ,0 ,0);
    Vector3D vy(0 ,1 ,0);
    Vector3D vz(0 ,0 ,1);
    Vector3D va(1 ,2 ,3);
    Vector3D v;

    AngleAxis aax(M_PI/2.0, 1, 0, 0);
    AngleAxis aay(M_PI/2.0, 0, 1, 0);
    AngleAxis aaz(grad2Rad(90), 0, 0, 1);

    PRINTF("Angle axix: \n");
    aax.print();
    aay.print();
    aaz.print();


    /********************************************/

    PRINTF("4 original vectors\n");
    vx.print();
    vy.print();
    vz.print();
    va.print();

    PRINTF("\n\neach rotated on x\n");
    v = vx.aRotate(aax); v.print();
    v = vy.aRotate(aax); v.print();
    v = vz.aRotate(aax); v.print();
    v = va.aRotate(aax); v.print();

    PRINTF("\n\neach rotated on y\n");
    v = vx.aRotate(aay); v.print();
    v = vy.aRotate(aay); v.print();
    v = vz.aRotate(aay); v.print();
    v = va.aRotate(aay); v.print();

    PRINTF("\n\neach rotated on z\n");
    v = vx.aRotate(aaz); v.print();
    v = vy.aRotate(aaz); v.print();
    v = vz.aRotate(aaz); v.print();
    v = va.aRotate(aaz); v.print();

    PRINTF(" we rotate 3x 120 degree axis 1,1,1)\n");
   
    AngleAxis aab(grad2Rad(120), 1,1,1);
    v = vx; v.print();
    v = v.aRotate(aab); v.print();
    v = v.aRotate(aab); v.print();
    v = v.aRotate(aab); v.print();
    v = v.aRotate(aab); v.print();


    PRINTF(" ----------------- TEST END ----------------\n");

}
