#include "rodos.h"
#include "matlib.h"

void MAIN(void) {

	Vector3D v(1 ,2 ,3);	  // from vector
	Vector3D w(0 , -1 , -4);  // to vector
	double alpha ;
	Vector3D u;

	alpha = v.getAngle(w); // liefert winkel in rad
	u = v.cross (w).normalize();

	AngleAxis u_phi(alpha, u);
	Quaternion q(u_phi);
	YPR ypr(q);

	PRINTF("Rotation q: "); q.print();
	PRINTF("Rotation ypr: "); ypr.print();

	PRINTF("----------- again --------------- \n");

	q = Quaternion(cos(alpha/2), u.scale(sin(alpha/2)));
	ypr = YPR(q);
	PRINTF("Rotation q: "); q.print();
	PRINTF("Rotation ypr: "); ypr.print();
}


