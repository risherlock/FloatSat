




#pragma once

#include "gateway/linkinterface.h"
#include "gateway/gateway.h"
#include "gateway/networkmessage.h"
#include "hal/hal_uart.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

/**
 * @file linkinterfaceuart.h
 * @date 2012/012/03 10:07
 * @author TW
 *
 *
 * @brief subscribers as gateway
 *
 */


#define MAX_UART_MESSAGE_LENGTH (MAX_NETWORK_MESSAGE_LENGTH +6)

#define OUTPUT_BUF_SIZE     64
#if OUTPUT_BUF_SIZE < 2
#error "linkinterface uart: output buffer to small"
#endif

class LinkinterfaceUART : public Linkinterface, IOEventReceiver  {

    HAL_UART* uart;
    //unsigned char inputBuffer[MAX_UART_MESSAGE_LENGTH]; // bom, topicid, message eom
    unsigned char* inputBuffer;

    /* used to send messages in a block and not char by char
     * -> important for bluetooth
     * -> if message is longer than outputBuffer it is split into multiple HAL_UART::write calls
     */
    char outputBuffer[OUTPUT_BUF_SIZE];

    enum ReadStates {
        WAITING_FOR_START,
        READING,
        ESCAPE
    } readState;
    int readIndex;

    volatile bool maySend; // controlled by ^s, ^q from reader
    bool enaTXBreak;  // enable transmit interruption by received character

    NetworkMessage* inMsg;
    char currCharFromLastCall;
    char prevChar;
    int inputIndex;
    bool foundUART_BOM;

    volatile bool transmitinProgrss;

    char getcharWaiting();
    void putcharEncoded(const bool mark, const char c);
    int putcharEncodedToBuf(char* buf, const bool mark, const char c);
    bool sendUartBuffer(char* buf, int size);
    bool processByte(char input);
    void myPutChar(char c);

public:

    LinkinterfaceUART(HAL_UART* uart, long int id = -1, bool enaTXBreak = true);
    void init();

    bool isNetworkMsgSent();
    bool sendNetworkMsg(NetworkMessage& outgoingMessage);
    bool getNetworkMsg(NetworkMessage &inMsg, int32_t &numberOfReceivedBytess);

    void onWriteFinished(); // from IOEventReceiver
    void onDataReady();  // from IOEventReceiver
};


#ifndef NO_RODOS_NAMESPACE
}
#endif

