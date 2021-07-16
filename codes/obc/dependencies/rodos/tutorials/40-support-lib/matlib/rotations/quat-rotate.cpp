#include "rodos.h"
#include "matlib.h"
#include "math.h"

void MAIN() {

	Vector3D v(1 ,2 ,3);
	Vector3D w(-2, -1 , -4);


        v = v.normalize();
        w = w.normalize();

	double alpha = v. getAngle (w); // liefert winkel in rad
	Vector3D u = v. cross (w);

	u = u.normalize();
	Quaternion q(cos(alpha/2), u.scale(sin(alpha/2)));
	//q = q.normalize();

	u = v.qRotate(q);
	PRINTF("rotate v : "); v.print();
	PRINTF("    to w : "); w.print();
        PRINTF("result   : "); u.print();

        PRINTF(" ----------- mit quat mult \n");

	Quaternion v2w = q * Quaternion(0, v) * -q; 
	PRINTF("rotate v : "); v.print();
	PRINTF("    to w : "); w.print();
        PRINTF("result   : "); v2w.q.print();


        PRINTF(" ----------- zweite Version \n");
        alpha = v. getAngle (w); // liefert winkel in rad
        u = v. cross (w). normalize ();
        AngleAxis u_phi(alpha,u);
        Quaternion qq(u_phi);

	u = v.qRotate(qq);
	PRINTF("rotate v : "); v.print();
	PRINTF("    to w : "); w.print();
        PRINTF("result   : "); u.print();


}
