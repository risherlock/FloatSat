/*
 * syscalls template
 * Sergio Montenegro
 */


#include "rodos.h"

namespace RODOS {

void sp_partition_yield() { }
void FFLUSH()             { }
void enterSleepMode()     { }
void hwInitTime()         { }
//int getCurrentIRQ(){ return 0; }
unsigned long long hwGetNanoseconds() { return 0; }

void hwInit() {
    xprintf("\n\n This port is empty! Will do nothing, maybe only crash\n\n");
}

}
