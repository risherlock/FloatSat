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
    AngleAxis aaz(M_PI/2.0, 0, 0, 1);


    Quaternion qx(aax);
    Quaternion qy(aay);
    Quaternion qz(aaz);

    PRINTF("Quaternions: \n");
    qx.print();
    qy.print();
    qz.print();


    /********************************************/

    PRINTF("4 original vectors\n");
    vx.print();
    vy.print();
    vz.print();
    va.print();

    PRINTF("\n\neach rotated on x\n");
    v = vx.qRotate(qx); v.print();
    v = vy.qRotate(qx); v.print();
    v = vz.qRotate(qx); v.print();
    v = va.qRotate(qx); v.print();

    PRINTF("\n\neach rotated on y\n");
    v = vx.qRotate(qy); v.print();
    v = vy.qRotate(qy); v.print();
    v = vz.qRotate(qy); v.print();
    v = va.qRotate(qy); v.print();

    PRINTF("\n\neach rotated on z\n");
    v = vx.qRotate(qz); v.print();
    v = vy.qRotate(qz); v.print();
    v = vz.qRotate(qz); v.print();
    v = va.qRotate(qz); v.print();


    PRINTF("----------------------------------\n");
    PRINTF("rotationsquaternion x,y,z \n");
    Quaternion qall  = qz * qy * qx;
    qall.print();

    PRINTF("\n\neach rotated on x,y,z using one qauternion\n");
    v = vx.qRotate(qall); v.print();
    v = vy.qRotate(qall); v.print();
    v = vz.qRotate(qall); v.print();
    v = va.qRotate(qall); v.print();

    PRINTF("\n\neach rotated on x,y,z using 3 quaternions\n");
    v = vx; v = v.qRotate(qx); v = v.qRotate(qy); v = v.qRotate(qz); v.print();
    v = vy; v = v.qRotate(qx); v = v.qRotate(qy); v = v.qRotate(qz); v.print();
    v = vz; v = v.qRotate(qx); v = v.qRotate(qy); v = v.qRotate(qz); v.print();
    v = va; v = v.qRotate(qx); v = v.qRotate(qy); v = v.qRotate(qz); v.print();


    PRINTF(" ----------------- TEST END ----------------\n");

}
