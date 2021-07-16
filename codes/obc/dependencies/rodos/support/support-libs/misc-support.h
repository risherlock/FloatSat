
#pragma once



#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/**
 * @file Support.h
 * @date 2008/11/21 11:46
 * @author Sergio Montenegro
 *
 *
 */

/**
 * Checks and computes plausbible values for sensor data  and
 * sets (wirtes) the las variable: outputVal
 * It returns true if the value is palsible (possibleMin .. posibleMax)
 * else returns false and the outputVal will not be modified.
 ***/
bool plausibleValue(double sensor1, double sensor2, double possibleMin, double posibleMax, double &outputVal);

/** returns the integer value of a ascii string representing a decimal number eg "123" -> 123 */
int stoi(char* s);


#ifndef NO_RODOS_NAMESPACE
}
#endif


