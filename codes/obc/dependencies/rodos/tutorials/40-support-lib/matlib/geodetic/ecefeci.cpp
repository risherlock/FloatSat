#include "rodos.h"
#include "matlib.h"
#include "stdlib.h"

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


void MAIN(void) {
    PRINTF("TEST ECEF<->ECI\n");
    srand(static_cast<unsigned int>(NOW()));
    double x,y,z,utc;
    int i;
    utc = 0;
    for(i=0;i<100;i++){
      
        x = fRand(-1000e3,1000e3);
        y = fRand(-1000e3,1000e3);
        z = fRand(-1000e3,1000e3);
        Vector3D ecef(x,y,z);
        utc = fRand(0,3650*DAYS);
	PRINTF("ORIGINAL ECEF UTC: %.3f days\n",utc/DAYS);
	ecef.print();
   
	Vector3D eci = ecefToECI(ecef,(int64_t)utc);
	PRINTF("-> ECI \n");
	eci.print();
        
        PRINTF("->ECEF \n");
	eciToECEF(eci,(int64_t)utc).print();
        PRINTF("\n");
	
	
    }

}


