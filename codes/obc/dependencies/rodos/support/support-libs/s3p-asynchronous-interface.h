#pragma once

/*
 * S3P Message framing, 
 * @author: Sergio Montenegro, Sept 2019, 2021
 */

/** please read s3p-encoder-in-byte stream.pdf **/

#include <stdint.h>
#include "s3p-code.h"

namespace RODOS {

/*
 * Asynchronous : Triggered from UART/RadioLink
 */

/*
 * To encode an send a message.
 * Case 2: Asynchronous sending. The UART/Radiolink (lower SW layers) has the control
 * it is the caller and request data when it is ready to take a new byte. 
 * The Upcall/Virtual function is on the side of the upper SW layers (user)
 *
 */

/*
 * Triggered from UART. Data flow from application to UART (UART requests : get)   
 */

class S3pSenderAsyncrhonous : public S3pCode {

    // __________________________ UPCALL _________________ You shall override this method (your single interface)
    // ___________________________________________________ Die UART calls getByteOrNODATA which calls getDualByte
    // ___________________________________________________ which request data or commands from your application
  public:
    virtual bool     retriggerSend() { return false; }    // UART shall try to get data from app and send per UART
  private:
    virtual uint16_t getDualByte()   { return NODATA; }   // App. override to send on request from UART
    //___________________________________________________

    uint16_t pendingDualByte;
    bool     isABytePendig = false;

  public:
    // called from the UART/Radiolink (int. Service Routine) when ready to send a new byte
    // Return NODATA if there is nothing to send, else only one Byte to send

    uint16_t getByteOrNODATA() {

        if(isABytePendig)              { isABytePendig = false;  return lsb(pendingDualByte);}

        pendingDualByte = getDualByte();
        if(pendingDualByte == NODATA)  {                         return NODATA;              } // Warning: Afet this app sall call retriggerSend()
        if(isCommand(pendingDualByte)) { isABytePendig = true;   return msb(pendingDualByte);}

        // now, it has to be a data byte, eventually which has to be stuffed
        if(pendingDualByte == MARK) {
            pendingDualByte = STUFF;
            isABytePendig = true;
            return msb(STUFF);
        }
        // last chance: The normal case :) a single byte
        return lsb(pendingDualByte);
    }

};

/*
 * Triggered from UART. Data flow from application to UART (request/get)   
 */

//_______________________________________________________
class S3pMsgSenderAsyncrhonous : public S3pSenderAsyncrhonous {

  private:
    // __________________________ UPCALL _________________ Application shall override this method 
    virtual uint8_t* requestBufferToRead(int32_t &msgLen) { msgLen = 0; return 0; } // tell me from where to read and how many bytes
    //___________________________________________________

    int32_t   len  = 0;
    uint8_t*  msg  = 0;

    // internal function called from the S3pSenderAsyncrhonous, which is called from UART
    uint16_t getDualByte() {                  // called from the UART/Radiolink  -> getByteOrNODATA -> getDualByte
        if(msg == 0) {
            msg = requestBufferToRead(len); 
            if(msg == 0) return NODATA;     // in this case (no msg), remember to call retriggerSend() when you can send again
            else         return BOM;
        }
        if(len == 0) { msg = 0; return EOM; }

        len--;
        return (uint16_t)*msg++; // stuffing is not required here. see getByteOrNODATE() who calls getDualByte()
    }
};

/*
 * To receive messages from UART/Radiolink and decode.
 * Case 2: Asynchronous receiving. The UART/Radiolink (lower SW layers) has the control
 * it is the caller and provides data when it is ready to deliver a new byte
 * The Upcall/Virtual function is on the side of the upper SW layers (user)
 */


/*
 * Triggered from UART. Data flow from UART to application (UART calls  put)
 */

class S3pReceiverAsyncrhonous : S3pCode {
  private:
    // _________________________ UPCALL _________________ You shall override this method to read messages from the UART/Radiolink
    // __________________________________________________ all upcalls are called from putByte() when it finds:
    virtual uint8_t* requestBufferToWrite(int32_t &maxlen)      { maxlen = 0; return 0; } // tell me where to write the next message and max len
    virtual void     endOfMessage([[gnu::unused]] int32_t len)  {} // when one message is ready (EOM found). It gives you the length

    //____________________________________________________
    int32_t  len      =   -1;
    uint8_t* msg      =    0;
    int32_t  maxLen   = 1300; // just as example
    uint16_t dualByte =    0;

  public:
    void putByte(uint8_t inputByte) { // the lower SW layers (UART/Radiolink) calls this for each arrived byte

        if(msg == 0) msg = requestBufferToWrite(maxLen);  // maxLen shall be updated by you

        dualByte  = (uint16_t)(dualByte  << 8 ) | (uint16_t)inputByte;
        if(inputByte == MARK) return; // here the msb, next call with the lsb
        if(msb(dualByte) == 0 ) dualByte = 0; // It is data, 0 is just to have some thing for the case

        switch(dualByte) {
            case 0:        if(msg != 0 && len >= 0 && len < maxLen) msg[len++] = inputByte; break;
            case STUFF:    if(msg != 0 && len >= 0 && len < maxLen) msg[len++] = MARK;      break;
            case EOM:      if(len >= 0) { endOfMessage(len); msg = 0; len = -1; }           break;
            case BOM:      if(msg != 0) len = 0;                                            break;
            default: executeCommand(dualByte);
        }
        dualByte = 0;
    } // putByte
};

} // namespace RODOS

