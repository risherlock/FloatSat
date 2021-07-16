#include "rodos.h"
#include "matlib.h"
#include "stdlib.h"

/**
  This thread creates 1000 randomized polar coordinates and tests
  the conversion to ECEF and backwards.
*/
class Transformer : public StaticThread<>{
long max = 500E3; //m  
long c = 0;  
void init(){
     srand(static_cast<unsigned int>(NOW()));
     PRINTF("dh \t\t dlat \t\t dlon \n");
}
void run(){
     while(c<1000){

      double h,phi,lambda;
      phi = ((double)(rand() % max) * M_PI/max - M_PI/2)/2;
      lambda = (double)(rand() % max) * M_PI/max - M_PI/2;
      h = rand();
      //PRINTF("h %5.5f \t phi %4.9f \t lambda %4.9f \n",h,phi,lambda);
      Polar geodetic(h , phi , lambda);
      Vector3D ecef;
   
      ecef = geodeticToECEF(geodetic);
      
      Polar geodetic2 = ecefToGeodetic(ecef);
      double dh, dlat, dlon;
      dh   = RODOS::fabs(geodetic.r - geodetic2.r);
      dlat = RODOS::fabs(geodetic.phi - geodetic2.phi);
      dlon = RODOS::fabs(geodetic.theta - geodetic2.theta);
      PRINTF("%.5f \t %.9f \t %.9f \n",dh,dlat,dlon);
      c++;
    }
} }transformer;
    
