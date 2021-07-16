#include "rodos.h"
#include "matlib.h"

void MAIN() {

    Vector3D vx(1 ,0 ,0);
    Vector3D vy(0 ,1 ,0);
    Vector3D vz(0 ,0 ,1);
    Vector3D va(1 ,2 ,3);

    Matrix3D rx, ry, rz;
    rx.rotationX(M_PI/2.0);
    ry.rotationY(M_PI/2.0);
    rz.rotationZ(M_PI/2.0);

    Vector3D v;
    Matrix3D r;

    // if(vy == vx) PRINTF(" error in line 19\n");
    if(! vx.equals(vx)) PRINTF("error in line 20\n");

    /********************************************/

    PRINTF("4 original vectors\n");
    vx.print();
    vy.print();
    vz.print();
    va.print();

    PRINTF("normalized vectors\n");
    v = vx.normalize(); v.print();
    v = vy.normalize(); v.print();
    v = vz.normalize(); v.print();
    v = va.normalize(); v.print();

    PRINTF("\n\neach rotated on x\n");
    v = rx * vx; v.print();
    v = rx * vy; v.print();
    v = rx * vz; v.print();
    v = rx * va; v.print();

    PRINTF("\n\neach rotated on y\n");
    v = ry * vx; v.print();
    v = ry * vy; v.print();
    v = ry * vz; v.print();
    v = ry * va; v.print();

    PRINTF("\n\neach rotated on z\n");
    v = rz * vx; v.print();
    v = rz * vy; v.print();
    v = rz * vz; v.print();
    v = rz * va; v.print();


    PRINTF("----------------------------------\n");
    PRINTF("rotationematrix x,y,z\n");
    r = rz * ry * rx; r.print();

    PRINTF("\n\neach rotated on x,y,z using one matrix\n");
    v = r * vx; v.print();
    v = r * vy; v.print();
    v = r * vz; v.print();
    v = r * va; v.print();

    PRINTF("\n\neach rotated on x,y,z using 3 matrix\n");
    v = vx; v = rx * v; v = ry * v; v = rz * v; v.print();
    v = vy; v = rx * v; v = ry * v; v = rz * v; v.print();
    v = vz; v = rx * v; v = ry * v; v = rz * v; v.print();
    v = va; v = rx * v; v = ry * v; v = rz * v; v.print();


    PRINTF(" ----------------- TEST END ----------------\n");

}
