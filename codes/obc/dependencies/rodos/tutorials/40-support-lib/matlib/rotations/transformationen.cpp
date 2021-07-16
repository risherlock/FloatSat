#include "rodos.h"
#include "matlib.h"


void MAIN() {

    Vector3D p1(2, 3, 4);
    Vector3D p2;

    Vector3D u(1,2,3);
    u = u.normalize();
    AngleAxis aa(grad2Rad(77), u);

    Quaternion qq=aa.toQuaternion();

    aa = qq.toAngleAxis();
    p2 = p1.aRotate(aa);
    p2.print();

    Matrix3D mm = qq.toMatrix3D();
    p2 = p1.mRotate(mm);
    p2.print();

    
    p2 = p1.qRotate(qq);
    p2.print();

   YPR yy=qq.toYPR();
   aa = yy.toAngleAxis();
   p2 = p1.aRotate(aa);
   p2.print();
   p2 = p1.yprRotate(yy);
   p2.print();
}


