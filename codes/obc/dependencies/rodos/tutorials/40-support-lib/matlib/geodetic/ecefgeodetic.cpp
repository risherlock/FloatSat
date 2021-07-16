#include "rodos.h"
#include "matlib.h"
#include "stdlib.h"
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


void MAIN(void) {

    PRINTF("TEST ECEF->GEODETIC\n");
	
    srand(static_cast<unsigned int>(NOW()));	
    int i;
    double theta,phi,h;
    for(i=0;i<100;i++){
	theta = fRand(-M_PI,M_PI);
        phi = fRand(-M_PI/2,M_PI/2);
        h = fRand(10e3,1000e3);
	    PRINTF("\n\nORIGINAL\n");
	    Polar(h,phi,theta).print();
        PRINTF("RECONVERTED\n");
        Vector3D ecef= geodeticToECEF(Polar(h,phi,theta));
	    Polar geo    = ecefToGeodetic(ecef);
        PRINTF("ecef = ");
        ecef.print();
	
        PRINTF("geodetic = ");
        geo.print();

  
	
	
    }

}


