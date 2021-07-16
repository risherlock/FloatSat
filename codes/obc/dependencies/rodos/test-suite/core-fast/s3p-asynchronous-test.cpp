/*
 * S3P Encoder / decoder synchronous test, 
 * @Author: Sergio Montenegro, 2021
 */

#include "rodos.h"
#include "s3p-asynchronous-interface.h"

uint32_t printfMask = 0;

void printchar(uint8_t c) {
    if(c >= ' ' && c <= 'z') PRINTF("%c", c);
    else                     PRINTF("-%02x-", c);
}

Fifo<uint8_t, 1000> commChannel; // to simulate a communication  link from Node B to Node A


/**
 *  Asynchronous test:
 *  One UART/RadioLink request (per Interrupts) Bytes to send
 *  Another UART/RadioLink provides (per Interrupts) bytes to an upper application
 */

struct Messages {
    static const int MAX_LEN = 100;
    int len;
    uint8_t msg[MAX_LEN];
};

Messages msgToSend[4] = {
    {10, {0,1,2,3,4,5, S3pCode::MARK, 16,17,18,19 } },
    {29, "hello world how are you doing"},
    {15, "I am doing well"},
    {16, "and how are you?"} 
};

Messages msgToReceive;


//__________________________________________________________________________  This is your interface to the Applications
class ApplicationInterface : public S3pReceiverAsyncrhonous, public S3pMsgSenderAsyncrhonous {

    //_________________________ Simulate an Application which receives data in one node
    void endOfMessage(int32_t len) { 
        for(int i = 0; i < len; i++) printchar(msgToReceive.msg[i]); 
        PRINTF(":-- %d bytes end of msg\n", len); 
    }
    uint8_t* requestBufferToWrite(int32_t &maxlen) {
        PRINTF("new Message --:");
        maxlen = msgToReceive.MAX_LEN;
        return msgToReceive.msg;
    }

    //____________________ for Node B: Simulate Application which provides data to send
    int msgCnt = 0;
    uint8_t* requestBufferToRead(int32_t &msgLen) { 
        if(msgCnt >= 4) {
            msgCnt = 0;
            msgLen = 0;
            return   0;
        }
        msgLen     = msgToSend[msgCnt].len;
        uint8_t* s = msgToSend[msgCnt].msg; 
        msgCnt++;
        return s;
    }
} applicationInterface;


//_____________________________________________ in Node A: reads from UART and pass to application
class ReceivingUART : public StaticThread<> {

    //_________ this run() is like an interrupt server which reads an UART
    void run() {
        printfMask = 1;
        int emptyCounter = 0;
        uint8_t c;
        while(1) {
            PRINTF("______________________ Node A reads its input UART\n");
            while(commChannel.get(c)) applicationInterface.putByte(c); // This shall be done by UART driver
            suspendCallerUntil(NOW() + 1*SECONDS);
            if(commChannel.isEmpty()) emptyCounter++; // to wait 2 seconds after end of transmissions
            if(emptyCounter > 2) hwResetAndReboot();
        }
    }
} receivingUART;


//_____________________________________________ in Node B: When UART is ready to send, requests Data from applications
class SendingUART: public StaticThread<> {

    //_________ this run() is like an interrupt server which writes to an UART
    void run() {
        uint16_t dualByte;
        for(int i = 0; i < 5; i++) { // send 5 times the messages set
            while((dualByte = applicationInterface.getByteOrNODATA()) != S3pCode::NODATA) {
                commChannel.put((uint8_t)(0x00ff & dualByte));
            }
            // nor more data to send... Wait and simulate the sending application tries again
            suspendCallerUntil(NOW() + 1*SECONDS);
        }
    }
} sendingUART;

