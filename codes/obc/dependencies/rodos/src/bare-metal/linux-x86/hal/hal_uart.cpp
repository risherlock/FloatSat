/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg

 * All rights reserved.
 *
 * @author Michael Ruffer
 */
#include "hal/hal_uart.h"
#include "hal/hal_gpio.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> // for interrupt


#include "hw_udp.h"
#include "rodos.h"

#define MAX_READ_CHUNK_SIZE 16

namespace RODOS {

//================================================================================
//Mapping of UART IDs to linux device names
#define MAX_NUM_UARTS 8
const char* uartDeviceNames[MAX_NUM_UARTS] = { "/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/rfcomm0", "/dev/rfcomm1", "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyUSB2", "/dev/ttyUSB3" };
//================================================================================


void uart_sig_io_handler(int);


bool signal_init = false;
void sig_io_handler(int);

int uart_rxErrorStatus(int idx);

class HW_HAL_UART {
  public:
    bool charReady;
    char lastChar;
    int  fd;

    UART_IDX  idx;
    HAL_UART* hal_uart;
};


HW_HAL_UART UART_contextArray[MAX_NUM_UARTS];

// constructor with pin assignment - dummy on Linux
HAL_UART::HAL_UART(UART_IDX uartIdx, GPIO_PIN, GPIO_PIN, GPIO_PIN, GPIO_PIN) : HAL_UART(uartIdx) {
}

HAL_UART::HAL_UART(UART_IDX uartIdx) {
    context = &UART_contextArray[uartIdx];

    context->idx      = uartIdx;
    context->hal_uart = this;
    context->fd       = -1;
}

/**
 * Maps a given baudrate to a c_cflags speed constant
 * (Basically the magic constants that you have to give the
 * cfsetispeed function).
 * The POSIX constants are defined in bits/termios.h,
 * and the extra non-POSIX ones are from x86_64-linux-gnu/bits/termios-baud.h
 * Returns zero if the baudrate is not listed in here
 */
static speed_t standardBaudrateLookup(unsigned int baudrate) {

    switch(baudrate) {
        // POSIX standard baudrates
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;

        // extra non-POSIX baudrates
        // hidded behind ifdef checks so we dont fail to compile on
        // platforms where this is not supported

#ifdef B57600
        case 57600: return B57600;
#endif

#ifdef B115200
        case 115200: return B115200;
#endif

#ifdef B230400
        case 230400: return B230400;
#endif

#ifdef B460800
        case 460800: return B460800;
#endif

#ifdef B500000
        case 500000: return B500000;
#endif

#ifdef B576000
        case 576000: return B576000;
#endif

#ifdef B921600
        case 921600: return B921600;
#endif

#ifdef B1000000
        case 1000000: return B1000000;
#endif

#ifdef B1152000
        case 1152000: return B1152000;
#endif

#ifdef B1500000
        case 1500000: return B1500000;
#endif

#ifdef B2000000
        case 2000000: return B2000000;
#endif

#ifdef B2500000
        case 2500000: return B2500000;
#endif

#ifdef B3000000
        case 3000000: return B3000000;
#endif

#ifdef B3500000
        case 3500000: return B3500000;
#endif

#ifdef B4000000
        case 4000000: return B4000000;
#endif

        // nothing matched
        default: return 0;
    }
}


/*
 * USART
 * - all USART will be initialized in 8N1 mode
 */
int HAL_UART::init(uint32_t iBaudrate) {

    struct termios t; ///< control structure for a general asynchronous interface

    context->charReady = false;
    context->lastChar  = 0;

    const char* devname = uartDeviceNames[context->idx];

    /* prepare termios */
    t.c_iflag     = IGNBRK | IGNPAR;
    t.c_oflag     = 0;
    t.c_cflag     = CS8 | CREAD; // | CRTSCTS | CLOCAL;
    t.c_lflag     = 0;           //ICANON;
    t.c_cc[VMIN]  = 1;           //TW:/* wait for 1 byte */
    t.c_cc[VTIME] = 0;           //TW:/* turn off timer */

    speed_t speed = standardBaudrateLookup(iBaudrate);
    if(speed != 0) {
        // can use a "default" baudrate, no hacks required
        cfsetispeed(&t, speed);
        cfsetospeed(&t, speed);
    } else {
        // non-default baudrate, hacks would be required...
        // if you find yourself here, needing a non-default baudrate on linux,
        // maybe the following is of some help?
        // https://stackoverflow.com/a/19440269

        xprintf("UART: baudrate not supported: %d\n", iBaudrate);
        return -1;
    }

    int fd      = open(devname, O_RDWR | O_NDELAY);
    context->fd = fd;
    if(fd == -1) {
        xprintf("UART: cannot open file: %s\n", devname);
        return -1;
    }

    //cfmakeraw(&t);
    tcsetattr(fd, TCSANOW, &t);

    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, O_NONBLOCK | O_ASYNC);

    init_sigio_handler();

    //xprintf("UART: %s initialized with: %d\n",devname,iBaudrate);

    return 0;
}


int HAL_UART::config(UART_PARAMETER_TYPE type, int paramVal) {

    switch(type) {
        case UART_PARAMETER_BAUDRATE:
            if(paramVal > 0) {
                reset();
                init(static_cast<uint32_t>(paramVal));
            } else {
                return -1;
            }
            break;

        case UART_PARAMETER_HW_FLOW_CONTROL:
            return -1;

        case UART_PARAMETER_ENABLE_DMA:
            return -1;

        default:
            return -1;
    }

    return 0;
}


void HAL_UART::reset() {
    close(context->fd);
}


size_t HAL_UART::read(void* buf, size_t size) {
    size_t bytesRed = 0;

    if(size > 0) {
        ssize_t retval;
        retval = ::read(context->fd, buf, size);
        if(retval < 0) return 0;
        bytesRed = static_cast<size_t>(retval);
    }

    return bytesRed;
}


size_t HAL_UART::write(const void* buf, size_t size) {
    ssize_t retval;

    retval = ::write(context->fd, buf, size);
    if(retval < 0) {
        retval = 0;
    };

    return static_cast<size_t>(retval);
}


int16_t HAL_UART::getcharNoWait() {
    uint8_t c = 0;

    if(read(&c, 1) > 0) {
        return static_cast<int16_t>(c);
    } else {
        return -1;
    }
}


int16_t HAL_UART::putcharNoWait(uint8_t c) {
    if(write(&c, 1) > 0) {
        return static_cast<int16_t>(c);
    } else {
        return -1;
    }
}


int HAL_UART::status(UART_STATUS_TYPE type) {

    UART_IDX idx = context->idx;

    if((idx < 1) || (idx > 5)) { return -1; }

    switch(type) {
        case UART_STATUS_RX_BUF_LEVEL:
            return -1;

        case UART_STATUS_RX_ERROR:
            return -1;
        //...
        default: return -1;
    }
}


bool HAL_UART::isWriteFinished() {
    //return true;
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(context->fd, &fdset);

    timeval tval;
    tval.tv_sec  = 0;
    tval.tv_usec = 0;

    // check if  fd can be written
    if(select(context->fd + 1, 0, &fdset, 0, &tval) > 0)
        return true;
    else
        return false;
}


bool HAL_UART::isDataReady() {


    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(context->fd, &fdset);

    timeval tval;
    tval.tv_sec  = 0;
    tval.tv_usec = 0;

    // check if there is something to read on fd
    if(select(context->fd + 1, &fdset, 0, 0, &tval) > 0)
        return true;
    else
        return false;
}


void uart_sig_io_handler(int) {
    //PRINTF("UART IRQ\n");
    for(int i = 0; i < MAX_NUM_UARTS; i++) {
        HAL_UART* uart = UART_contextArray[i].hal_uart;
        if(uart) {
            if(uart->isDataReady()) {
                uart->upCallDataReady();
            }
            if(uart->isWriteFinished()) {
                uart->upCallWriteFinished();
            }
        }
    }
}


} // namespace RODOS
