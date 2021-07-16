/*
 * hal_can.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Erik
 */

//For Linux SocketCAN API

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <netdb.h>
// #include <netinet/in.

#include "hal/hal_can.h"
#include "rodos.h"


#include <new>

namespace RODOS {

#define MAX_FILTERS 20

//================================================================================
//Mapping of CAN IDs to linux can network device names
#define MAX_CAN_DEVICES 4
const char* canDeviceNames[MAX_CAN_DEVICES] = { "can0", "can1", "vcan0", "vcan1" };
//================================================================================

#define MAX_HALCAN_INSTANCES 16
int         numHalCanInstances = 0;
HW_HAL_CAN* activeDevices[MAX_HALCAN_INSTANCES];

void CANGlobalInit();
void can_sig_io_handler(int signo);

void CANGlobalInit() {
    static bool init = false;
    if(!init) {
        init = true;
    }
}


#define MAX_FILTERS 20

struct CAN_Filter {
    uint32_t    canID;
    uint32_t    canIDmask;
    bool        isExtID;
    bool        rtr;
    HAL_CAN*    owner;
    HW_HAL_CAN* owner_context;
};


class HW_HAL_CAN {
  public:
    const char* devName;
    int         s;

    can_filter filters[MAX_FILTERS];
    uint32_t    numFilters;

    Fifo<can_frame, 64> RxFifo;
    volatile bool       rxFifoEmpty;

    HAL_CAN* hal_can;


  public:
    HW_HAL_CAN();

    void setupFilters();
};


HW_HAL_CAN::HW_HAL_CAN() {
}

void HW_HAL_CAN::setupFilters() {

    if(numFilters > 0) {
        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, filters, static_cast<socklen_t>(sizeof(can_filter) * numFilters));
    } else {
        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    }
}


HAL_CAN::HAL_CAN(CAN_IDX canIdx, GPIO_PIN, GPIO_PIN) {
    context              = (HW_HAL_CAN*)xmalloc(sizeof(HW_HAL_CAN));
    RODOS_ASSERT_IFNOT_RETURN_VOID(context != nullptr);
    context              = new(context) HW_HAL_CAN();
    context->devName     = canDeviceNames[canIdx];
    context->rxFifoEmpty = true;
    context->s           = 0;
    context->hal_can     = this;
    RODOS_ASSERT_IFNOT_RETURN_VOID(numHalCanInstances < MAX_HALCAN_INSTANCES); // Too many HAL_CAN's, see  MAX_HALCAN_INSTANCES

    activeDevices[numHalCanInstances] = context;
    numHalCanInstances++;
}

int HAL_CAN::init(unsigned int) {

    if(context->s > 0) {
        close(context->s);
    }

    struct sockaddr_can addr;
    struct ifreq        ifr;

    if((context->s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        return -1;
        PRINTF("socket fail\n");
    }

    strcpy(ifr.ifr_name, context->devName);
    ioctl(context->s, SIOCGIFINDEX, &ifr);
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if(bind(context->s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return -1;
        PRINTF("bind fail\n");
    }

    init_sigio_handler();

    fcntl(context->s, F_SETFL, O_NONBLOCK);
    fcntl(context->s, F_SETOWN, getpid());
    /* turn on async mode -- this is the important part which enables signal delivery */
    fcntl(context->s, F_SETFL, fcntl(context->s, F_GETFL, 0) | O_ASYNC);

    context->setupFilters();

    return 0;
}

void HAL_CAN::reset() {
    close(context->s);
    context->s = 0;
}

int HAL_CAN::config(CAN_PARAMETER_TYPE type, uint32_t) {
    switch(type) {
        case CAN_PARAMETER_BAUDRATE:

            return -1;
    }
    return -1;
}


CanErrorMsg HAL_CAN::status(CAN_STATUS_TYPE type) {
    CanErrorMsg errorMsg;
    errorMsg.errorType   = CAN_ERROR_TYPE::UNKNOWN;
    errorMsg.canErrorCnt = 0;

    switch(type) {
        case CAN_STATUS_RX_ERROR:
            return errorMsg;
        case CAN_STATUS_TX_ERROR:
            return errorMsg;

        case CAN_STATUS_RX_LEVEL:
            return errorMsg;
    }
    return errorMsg;
}


bool HAL_CAN::isWriteFinished() {
    //	return true;
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(context->s, &fdset);

    timeval tval;
    tval.tv_sec  = 0;
    tval.tv_usec = 0;

    // check if  fd can be written
    if(select(context->s + 1, 0, &fdset, 0, &tval) > 0)
        return true;
    else
        return false;
}

bool HAL_CAN::isDataReady() {
    return !context->rxFifoEmpty;
}

bool HAL_CAN::addIncomingFilter(uint32_t ID, uint32_t IDMask, bool extID, bool rtr) {

    if(context->numFilters >= MAX_FILTERS)
        return false;


    context->filters[context->numFilters].can_id   = (extID ? (ID & 0x1FFFFFFF) : (ID & 0x7FF)) & ~CAN_ERR_FLAG;
    context->filters[context->numFilters].can_mask = ~(extID ? (IDMask) : (IDMask | 0xFFFFF800));
    context->filters[context->numFilters].can_mask |= 0xE0000000;
    context->filters[context->numFilters].can_mask &= ~(CAN_ERR_FLAG);

    context->filters[context->numFilters].can_id |= (rtr ? CAN_RTR_FLAG : 0) | (extID ? CAN_EFF_FLAG : 0);

    context->numFilters++;

    context->setupFilters();

    return true;
}


int8_t HAL_CAN::write(const uint8_t* sendBuf, uint8_t len, uint32_t canID, bool extID, bool rtr) {

    can_frame f;
    if(len > 8) return -1;

    f.can_id = extID ? (canID & 0x1FFFFFFF) : (canID & 0x7FF);
    f.can_id |= (rtr ? CAN_RTR_FLAG : 0) | (extID ? CAN_EFF_FLAG : 0);
    f.can_dlc = len;

    for(int i = 0; i < len; i++) {
        f.data[i] = sendBuf[i];
    }

    if(::write(context->s, &f, sizeof(f)) <= 0) {
        return -1;
    }

    return 0;
}


int8_t HAL_CAN::read(uint8_t* recBuf, uint32_t* canID, bool* isExtID, bool* rtr) {
    can_frame msg;

    bool _extID, _rtr;

    if(context->RxFifo.get(msg)) {

        _extID = (msg.can_id & CAN_EFF_FLAG) > 0;
        _rtr   = (msg.can_id & CAN_RTR_FLAG) > 0;


        if(canID) {
            *canID = _extID ? (msg.can_id & 0x1FFFFFFF) : (msg.can_id & 0x7FF);
        }
        if(isExtID) {
            *isExtID = _extID;
        }
        if(rtr) {
            *rtr = _rtr;
        }

        for(int i = 0; i < 8; i++) {
            recBuf[i] = msg.data[i];
        }

        return static_cast<int8_t>(msg.can_dlc);
    }

    context->rxFifoEmpty = true;
    return -1;
}


void can_sig_io_handler(int) {
    //PRINTF("UART IRQ\n");
    can_frame f;
    bool      dataReady;

    for(int i = 0; i < numHalCanInstances; i++) {
        if(activeDevices[i] && activeDevices[i]->s > 0) {
            dataReady = false;
            while(::read(activeDevices[i]->s, &f, sizeof(f)) > 0) {
                activeDevices[i]->RxFifo.put(f);
                activeDevices[i]->rxFifoEmpty = false;
                dataReady                     = true;
            }
            if(dataReady) {
                activeDevices[i]->hal_can->upCallDataReady();
            }

            if(activeDevices[i]->hal_can->isWriteFinished()) {
                activeDevices[i]->hal_can->upCallWriteFinished();
            }
        }
    }
}


} // namespace RODOS
