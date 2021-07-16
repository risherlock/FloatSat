
#include "rodos.h"
#include "misc-support.h"

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/**
* @file support.cc
* @date 2009/01/21
* @author Sergio Montenegro
*
*
* @brief Misc functions
*
*/


/** Checks and computes plausbible values for sensor data **/
bool plausibleValue(double sensor1, double sensor2, double possibleMin, double posibleMax, double &outVal) {
    bool ok1 =  (sensor1 >= possibleMin)  &&  (sensor1 <= posibleMax);
    bool ok2 =  (sensor2 >= possibleMin)  &&  (sensor2 <= posibleMax);

    /***** correct impossible values ***/
    if (! ok1) sensor1 = sensor2;
    if (! ok2) sensor2 = sensor1;

    if (ok1 || ok2) 	outVal = (sensor1 + sensor2)/2.0;

    return ok1 || ok2;
}


int stoi(char* s) {
   int result = 0;
   while(*s >= '0' && *s <= '9') {
      result = result*10 + *s -'0';
      s++;
    }
   return result;
}


#ifndef NO_RODOS_NAMESPACE
}
#endif

