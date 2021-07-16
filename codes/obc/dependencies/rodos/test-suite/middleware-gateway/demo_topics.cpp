#include "rodos.h"

/** topic id == -1 -> generate a number from name, else take the nr **/

Topic<int32_t>  int32top(-1, "int32top");
Topic<int64_t>  int64top(-1, "int64top");
Topic<double>   doubltop(-1, "doulbtop");


