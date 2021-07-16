


/**
* @file hw_uart.h
* @date 2008/12/03 9:55
* @author Lutz Dittrich, Sergio Montenegro
*
*
* @brief UART communication
*
*/

#pragma once


#include "../api/putter.h"
namespace RODOS {

class UART : public Putter {

    int  fd; /* file descriptor */
    bool charReady;
    char lastChar;

  public:
    UART(const char* devname);
    ~UART();
    void         writechar(const char c);
    bool         isCharReady();
    bool         getcharNoWait(char& c);
    virtual bool putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo);
};


} // namespace RODOS
