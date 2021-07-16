

/**
* @file interruptservers.cc
* @date 2011/12/22 17:08
* @author Sergio Montenegro
*
*/
#include "topic.h"


namespace RODOS {

/*********************************************/
void sigtermHandler(int sig);
void uartHandler(int sig);
void timerHandler(int sig);


void sigtermHandler(int)   { interruptSigterm.publishFromInterrupt(0); }
void uartHandler(int)      { interruptUart.publishFromInterrupt(0); }
void timerHandler(int)     { interruptTimer.publishFromInterrupt(0); }


}

