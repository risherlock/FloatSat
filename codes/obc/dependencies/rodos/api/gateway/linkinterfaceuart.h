

#pragma once

#include "gateway/linkinterface.h"
#include "gateway/gateway.h"
#include "gateway/networkmessage.h"
#include "hal/hal_uart.h"


namespace RODOS {

/**
 * @file linkinterfaceuart.h
 * @date 2012/012/03 10:07
 * @author TW
 *
 *
 * @brief subscribers as gateway
 *
 */


constexpr int MAX_UART_MESSAGE_LENGTH = (MAX_NETWORK_MESSAGE_LENGTH + 6);

constexpr size_t OUTPUT_BUF_SIZE = 64;
static_assert(OUTPUT_BUF_SIZE > 1, "linkinterface uart: output buffer to small");

class LinkinterfaceUART : public Linkinterface, IOEventReceiver  {

    HAL_UART* uart;
    //unsigned char inputBuffer[MAX_UART_MESSAGE_LENGTH]; // bom, topicid, message eom
    uint8_t* inputBuffer;

    /* used to send messages in a block and not char by char
     * -> important for bluetooth
     * -> if message is longer than outputBuffer it is split into multiple HAL_UART::write calls
     */
    uint8_t outputBuffer[OUTPUT_BUF_SIZE];

    enum ReadStates {
        WAITING_FOR_START,
        READING,
        ESCAPE
    } readState;
    int readIndex;

    volatile bool maySend; // controlled by ^s, ^q from reader
    bool enaTXBreak;  // enable transmit interruption by received character

    NetworkMessage* inMsg;
    uint8_t currCharFromLastCall;
    uint8_t prevChar;
    int inputIndex;
    bool foundUART_BOM;

    volatile bool transmitinProgrss;

    char getcharWaiting();
    void putcharEncoded(const bool mark, const uint8_t c);
    size_t putcharEncodedToBuf(uint8_t* buf, const bool mark, const uint8_t c);
    bool sendUartBuffer(uint8_t* buf, size_t size);
    bool processByte(uint8_t input);
    void myPutChar(uint8_t c);

public:

    LinkinterfaceUART(HAL_UART* uart, int64_t id = -1, bool enaTXBreak = true);
    void init();

    bool isNetworkMsgSent();
    bool sendNetworkMsg(NetworkMessage& outgoingMessage);
    bool getNetworkMsg(NetworkMessage &inMsg, int32_t &numberOfReceivedBytess);

    void onWriteFinished(); // from IOEventReceiver

    virtual void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME);
};


}  // namespace


