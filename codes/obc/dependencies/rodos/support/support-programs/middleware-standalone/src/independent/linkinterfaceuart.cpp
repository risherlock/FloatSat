



#include "gateway/linkinterfaceuart.h"
#include "gateway/networkmessage.h"
#include "rodos-middleware.h"

/**
 * @file linkinterfaceuart.cc
 * @date 2012/10/26
 * @author Thomas Walter, uni Würzburg
 *
 * Link Interface to uart.
 *
 *
 */

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

/*********************************************************************************/

static const char  UART_BOM		=  0x02;
static const char  UART_EOM		=  0x03;
static const char  ENCODED_FF		=  0x7E;
static const char  STOP_MARK		=  0x18;
static const char  CONTINUE_MARK	=  0x01;
static const char  MARK			=  (char)0xff;

/*********************************************************************************/

LinkinterfaceUART::LinkinterfaceUART(HAL_UART* uart, long int id, bool enaTXBreak) : Linkinterface(id) {
    this->uart=uart;
    // this->uart->init();
    readState = WAITING_FOR_START;
    readIndex = 0;
    maySend = true;
    currCharFromLastCall=0;
    prevChar=0;
    inputIndex=0;
    foundUART_BOM=false;
    transmitinProgrss=false;
    this->enaTXBreak = enaTXBreak;
}

/*********************************************************************************/
void LinkinterfaceUART::putcharEncoded(const bool mark, const char c) {

    /** first check if we were stoped by control flow commands **/

    //for(int i = 0; i < 5 && !maySend; i++) // ^s-> stop ? but with a limit
     //   Thread::suspendCallerUntil(NOW() + 200*MILLISECONDS); // no more than 5 times, (one second)

    maySend = true; // in case the receiver is dead.

    /*** By the way, in linux do not need to send stop/continues...... */
    /** Not stoped -> send now **/
    if(mark) {
        myPutChar(MARK);
        myPutChar(c);
        return;						// <------------------------
    }

    if(c == MARK)  { // the only char for stuffing
        myPutChar(MARK);
        myPutChar(ENCODED_FF);
        return;						// <------------------------
    }
    /** nothing special, the char may go out without encoding **/
    myPutChar(c);

}

void LinkinterfaceUART::myPutChar(char c) {

        while(uart->putcharNoWait(c) == -1){
        	uart->suspendUntilWriteFinished();
        }
}


/*********************************************************************************/
int LinkinterfaceUART::putcharEncodedToBuf(char* buf, const bool mark, const char c) {

    if(mark) {
        *buf++ = MARK;
        *buf = c;
        return 2;                     // <------------------------
    }

    if(c == MARK)  { // the only char for stuffing
        *buf++ = MARK;
        *buf = ENCODED_FF;
        return 2;                     // <------------------------
    }
    /** nothing special, the char may go out without encoding **/
    *buf = c;
    return 1;
}
/*********************************************************************************/

bool LinkinterfaceUART::sendNetworkMsg(NetworkMessage& outgoingMessage)	{

    transmitinProgrss=true;
    int lenToSend = outgoingMessage.numberOfBytesToSend();
    char* dataToSend = (char*)&outgoingMessage;

    if (enaTXBreak){ // transmit interruption by received character is enabled
        putcharEncoded(true, UART_BOM);
        for(int i = 0; i < lenToSend; i++) {
            putcharEncoded(false,dataToSend[i] );
        }
        putcharEncoded(true, UART_EOM);
    }else{ // transmit interruption by received character is disabled -> we can send longer blocks (only important for BT)
        int wrIdx = 0;
        wrIdx += putcharEncodedToBuf(outputBuffer,true, UART_BOM);

        for(int i = 0; i < lenToSend; i++) {
            wrIdx += putcharEncodedToBuf(&outputBuffer[wrIdx],false,dataToSend[i] );

            if(wrIdx >= OUTPUT_BUF_SIZE-2){ // "-2" -> we need some space for UART_EOM
                if (!sendUartBuffer(outputBuffer,wrIdx)) {
                    PRINTF("Linkinterface Uart transmit error\n");
                    return false;
                }
                wrIdx = 0;
            }
        }
        wrIdx += putcharEncodedToBuf(&outputBuffer[wrIdx],true, UART_EOM);
        if (!sendUartBuffer(outputBuffer,wrIdx)) {
            PRINTF("Linkinterface Uart transmit error\n");
            return false;
        }
    }

    return true;
}


bool LinkinterfaceUART::sendUartBuffer(char* buf, int size){
    int txBytes = 0;
    int retVal = 0;
    int errCnt = 0;
    while (txBytes < size){
        retVal = uart->write(&buf[txBytes],size-txBytes);
        if (retVal <= 0){
            errCnt++;
            if (errCnt > 100){
                return false;
            }
            //Thread::suspendCallerUntil(NOW() + 1*MILLISECONDS);
        }else if ( retVal != (size-txBytes) ){ // the whole buffer couldn't be sent -> wait a moment and send the rest
            //Thread::suspendCallerUntil(NOW() + 1*MILLISECONDS);
            txBytes += retVal;
            errCnt = 0;
        }else{
            return true;
        }
    }

    return true;
}

/************************************
 * UART Gateway                     *
 * This methode is for every incoming byte. It collects all recieved character and
 * decodes them to the S3P protocol. Once the EOM is recognized, the data is stored
 * in a NetworkMessage frame and it returns true when the message is complete.
 ************************************/

bool LinkinterfaceUART::processByte(char inputChar) {

    prevChar = currCharFromLastCall;
    currCharFromLastCall = inputChar;

    if(inputChar == MARK)	{ return false; } 	// <------ 	fetch next char, ff alone does not help

    if(prevChar == MARK && inputChar == UART_EOM) {	  // end of message detected
        foundUART_BOM = false;
        return true;
    }

    if(prevChar == MARK && inputChar == UART_BOM) {		//Begin of networkmessage detected
        foundUART_BOM = true;
        inputIndex = 0;
        return false;
    }
    if(prevChar == MARK && inputChar == STOP_MARK) {
        maySend = false;
        return false;
    }
    if(prevChar == MARK && inputChar == CONTINUE_MARK) {
        maySend = true;
        return false;
    }
    if(prevChar == MARK && inputChar == ENCODED_FF) {
        if(foundUART_BOM) {
            inputBuffer[inputIndex] = MARK;
            if(inputIndex < MAX_UART_MESSAGE_LENGTH - 1 ) inputIndex++;
        }
        return false;
    }

    if(prevChar == MARK)  return false; 	// this is an unknown code! just discard
    if(currCharFromLastCall == MARK) return false; 	// <------ 	fetch next char, ff alone does not help

    inputBuffer[inputIndex] = inputChar;	// place recieved char in inputbuffer
    if(inputIndex < MAX_UART_MESSAGE_LENGTH - 1) inputIndex++;

    return false;
}


void LinkinterfaceUART::init() { uart->setIoEventReceiver(this); }

bool LinkinterfaceUART::isNetworkMsgSent() { return !transmitinProgrss; }

bool LinkinterfaceUART::getNetworkMsg(NetworkMessage& inMsg,int32_t &numberOfReceivedBytes) {
    inputBuffer = (unsigned char*)&inMsg;
    while(uart->isDataReady()) {
        char c = uart->getcharNoWait();
        if(processByte(c)) {
            numberOfReceivedBytes=inputIndex;
            return true;
        }
    }
    return false;
}

void LinkinterfaceUART::onWriteFinished() {
    transmitinProgrss=false;
    //if(threadToResume) threadToResume->resume();
}

void LinkinterfaceUART::onDataReady() {
   // if(threadToResume) threadToResume->resume();
}

#ifndef NO_RODOS_NAMESPACE
}
#endif
