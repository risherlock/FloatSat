#include "rodos.h"
#include "matlib.h"

void MAIN(void) {

	Vector3D v(1, 0, 0);	  // from vector
	Vector3D w(0, 1, 0);  // to vector

        Rotor rot(v, w);

        RPY rpy(rot);
        PRINTF("rotor: "); rot.print();
	PRINTF("rpy:   "); rpy.print();
}


