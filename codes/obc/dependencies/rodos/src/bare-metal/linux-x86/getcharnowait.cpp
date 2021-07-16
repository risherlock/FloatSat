


#include "rodos.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
//#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

namespace RODOS {

static bool initialized = false;

/* 
 * Getchar as interrupt: Signal
 */

static char inputBuf[500];
static void charReader(int) {
    fd_set         read_set;
    struct timeval timeout_value;

    GenericMsgRef msgRef;
    msgRef.msgPtr = inputBuf;

    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);

    timeout_value.tv_sec  = 0;
    timeout_value.tv_usec = 0;

    if(select(STDIN_FILENO + 1, &read_set, NULL, NULL, &timeout_value) > 0) {
        msgRef.msgLen           = static_cast<int32_t>(read(STDIN_FILENO, inputBuf, 100));
        inputBuf[msgRef.msgLen] = 0;
        charInput.publishFromInterrupt(&msgRef, sizeof(GenericMsgRef));
    }
}


void activateTopicCharInput() {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = charReader;
    sigaction(SIGIO, &sa, NULL);
    fcntl(0, F_SETOWN, getpid());
    fcntl(0, F_SETFL, O_NONBLOCK | O_ASYNC);

    // initialized = true;
}


/*
 * nonblocking getc / getchar (from stdin)
 */
void setNoDelay();
void setNoDelay() {
    struct termios term;

    //fcntl(kb,F_SETFL,fcntl(kb,F_GETFL,0)|O_NDELAY);
    fcntl(0, F_SETFL, O_NONBLOCK | O_NDELAY);
    tcgetattr(0, &term); // 0 is stdin

    term.c_lflag &= ~static_cast<unsigned>(ICANON);
    term.c_cc[VTIME] = 0;
    term.c_cc[VMIN]  = 0;
    tcsetattr(0, TCSANOW, &term); // 0 is stdin

    initialized = true;
}

/****************************************************/

static char lastChar = -1;

/** Returns -1 if no data, else char from getchar **/
char getcharNoWait() {
    if(initialized == false) setNoDelay();
    if(lastChar >= 0) {
        char c   = lastChar;
        lastChar = -1;
        return c;
    }
    return static_cast<char>(getchar());
}

bool isCharReady() {
    if(initialized == false) setNoDelay();
    if(lastChar >= 0) return true;
    lastChar = static_cast<char>(getchar());
    return (lastChar >= 0);
}


char* getsNoWait() {
    static char inBuffer[122];
    static int  inBufferIndex = 0;

    while(1) { // terminated with returns

        char c = getcharNoWait();
        if(c < 0) return 0;

        if(c == '\n') { // end of string -> return
            inBuffer[inBufferIndex] = 0;
            inBufferIndex           = 0;
            return inBuffer;
        }

        inBuffer[inBufferIndex] = c;
        if(inBufferIndex < 120) inBufferIndex++;
    }
}

} // namespace RODOS
