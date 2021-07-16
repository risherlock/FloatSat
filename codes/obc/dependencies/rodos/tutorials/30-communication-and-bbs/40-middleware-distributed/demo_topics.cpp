
#include "rodos.h"




/** Example topics for the tutorials **/

/** topic id == -1 -> generate a number from name, else take the nr **/

Topic<long>   counter1(-1, "counter1");
Topic<long>   counter2(-1, "counter2");
Topic<long>   counter3(-1, "counter3");
Topic<long>   counter4(-1, "counter4", true); // this topic is only local
Topic<long>   counter5(1002, "counter5");
Topic<long>   counter6(1003,"counter6");


