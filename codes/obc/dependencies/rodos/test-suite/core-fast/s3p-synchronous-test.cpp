/*
 * S3P Encoder / decoder synchronous test, 
 * @Author: Sergio Montenegro, 2021
 */

#include "rodos.h"
#include "s3p-synchronous-interface.h"

uint32_t printfMask = 0;

void printchar(uint8_t c) {
    if(c >= ' ' && c <= 'z') PRINTF("%c", c);
    else                     PRINTF("-%02x-", c);
}

Fifo<uint8_t, 1000> commChannel; // to simulate a communication  link from Node B to Node A 

/**
 *  Synchronous test:
 *  An application sends messages in a frame and writes to UART/RadioLink
 *  Another Application reads frames and gets byte for byte form the UART/RadioLink
 */

//__________________________________________________________________________  This is yout interface to the UART/RadioLink
class UartSim : public S3pSenderSyncrhonous, public S3pReceiverSyncrhonous {
    void    putByte(uint8_t c) { commChannel.put(c); }                       // Write to UART/RadioLink
    uint8_t getByte() { uint8_t c; while(!commChannel.get(c)) {} return c; } // Warning loop! get byte for byte from the UART/RadioLink
} uartSim;

//_________________________________________________________________________ Node B: Test sender
void sendBytes(int len) { // not in a message frame! Trash!
    for(uint8_t i = 0; i < len / 2; i++) uartSim.putDataByte(i);
    for(uint8_t i = 0; i < len / 2; i++) uartSim.putDataByte((uint8_t)(i+S3pCode::MARK-2)); // Test stuffing
}
void sendTextMsgs() {
    uartSim.sendMsg(29, (const uint8_t*)"hello world how are you doing");
    uartSim.sendMsg(15, (const uint8_t*)"I am doing well");
    uartSim.sendMsg(16, (const uint8_t*)"and how are you?");
}
void sendBinaryMsg(int len) {
    uartSim.putCommand(S3pCode::BOM);
    sendBytes(len);
    uartSim.putCommand(S3pCode::EOM);
}

//_____________________________________________  Simulate ona application which sends and one which receives 
class S3pTestApplicationSide : public StaticThread<> {

    //__________________ The one which sends
    void sendProbeMsgs() {
        sendBytes(10); // This shall not be inside a message frame
        sendTextMsgs();
        sendBytes(10); // This shall not be inside a message frame
        sendBinaryMsg(20);
    }

    //__________________ Node A: The one which receives 
    void run() {
        printfMask = 1;

        PRINTF("______________________ raw content of transmission channel, only to see the coding\n");
        sendProbeMsgs();
        PRINTF(" Raw Content --:");
        uint8_t c;
        while(commChannel.get(c)) printchar(c);
        PRINTF(":--\n");

        PRINTF("______________________ Real Application interface. We expect 4 Messages\n");
        uint8_t inputMessage[100];
        int     len = 0;
        sendProbeMsgs();
        for(int i = 0; i < 4; i++) {
            len = uartSim.getMsg(100, inputMessage);
            PRINTF(" msg %d (%d bytes): --:", i, len);
            for(int j = 0; j < len; j++) printchar(inputMessage[j]);
            PRINTF(":--\n");
        }
        PRINTF("commChannel.isEmpty()=%d (shall be 1)\n", commChannel.isEmpty());

        hwResetAndReboot();
    }

} s3pTestApplicationSide;
