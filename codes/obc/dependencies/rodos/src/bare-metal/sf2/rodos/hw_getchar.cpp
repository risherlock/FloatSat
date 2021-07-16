/**
 * @file hw_getchar.cpp
 * @date ?
 * @port 2017/07/14 19:14
 * @author modified by Martin Boekhoff, Christian Feichtinger
 *
 * @brief all specific stuff for uart
 *
 */
#include "rodos.h"

namespace RODOS {

extern HAL_UART uart_stdout;

// Attention:
// does return 0 if no character is available,
// because char is unsigned on ARM
char getcharNoWait() {
    int c = uart_stdout.getcharNoWait();
    if(c < 0) {
        return 0;
    } else {
        return static_cast<char>(c);
    }
}

bool isCharReady() {
    return uart_stdout.isDataReady();
}

char* getsNoWait() {
    static char inBuffer[122];
    static int  inBufferIndex = 0;

    while(1) { // terminated with returns

        char c = getcharNoWait();
        if(c == 0) return 0;

        if(c == '\r' || c == '\n') { // end of string -> return
            inBuffer[inBufferIndex] = 0;
            inBufferIndex           = 0;
            return inBuffer;
        }

        inBuffer[inBufferIndex] = c;
        if(inBufferIndex < 120) inBufferIndex++;
    }
}

void activateTopicCharInput() {}

} /* namespace RODOS */
