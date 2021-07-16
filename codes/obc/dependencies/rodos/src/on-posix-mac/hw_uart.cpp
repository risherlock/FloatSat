


/**
 * @file hw_uart.cc
 * @date 2008/12/03 10:03
 * @author Lutz Dittrich, Sergio Montenegro
 *
 *
 * @brief UART communication
 *
 */

#include "rodos.h"
#include "hw_uart.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
//#include <stdlib.h>
#include <unistd.h>

namespace RODOS {

UART::UART(const char* devname) {
    struct termios t;

    charReady = false;
    lastChar  = 0;

    /* prepare termios */
    t.c_iflag = IGNBRK | IGNPAR;
    t.c_oflag = 0;
    t.c_cflag = CS8 | CREAD | CLOCAL;
    t.c_lflag = 0;
    cfsetispeed(&t, B9600); /* normal shall be: B115200 Baud */
    cfsetospeed(&t, B9600);

    fd = open(devname, O_RDWR | O_NDELAY);
    RODOS_ASSERT(fd != -1);  // UART: cannot open file
    tcsetattr(fd, TCSANOW, &t);
}

UART::~UART() {
    close(fd);
}


static int notUsed;
void       UART::writechar(const char c) {
    notUsed = write(fd, &c, 1);
}

bool UART::isCharReady() {

    if(charReady) return true;

    charReady = getcharNoWait(lastChar);
    return charReady;
}

bool UART::getcharNoWait(char& c) {
    if(charReady) {
        charReady = false;
        c         = lastChar;
        return true;
    }

    char buf[1];
    if(read(fd, buf, 1) != 1) return false;
    c = buf[0];
    return true;
}

bool UART::putGeneric([[gnu::unused]] const long topicId, const unsigned int len, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
    unsigned int ctr;
    for(ctr = 0; ctr < len; ctr++) {
        writechar(((const char*)msg)[ctr]);
    }
    return true;
}

} // namespace RODOS
