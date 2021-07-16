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
 * Triggered from Application, Data Flow from Application to UART/RadioLink (Send/put)
 */

/*
 * To encode an send a message. 
 * Case 1: Synchronous sending. The calling function (upper SW layers) has the control
 * the sending channel is always ready to get data, e.g. from memory to memory,
 * or else busy waiting  until the sending channel (UART/Radiolink) is ready to get data.
 * The Upcall/Virtual function is on the side of the UART/Radiolink (lower SW layers)
 */


// TODO: Return and propagate Error codes
class S3pSenderSyncrhonous : public S3pCode {

  private:
    // _________________________ UPCALL _________________ You shall override this method to write to the UART/Radiolink
    virtual void putByte([[gnu::unused]] uint8_t c) {} // override to send or store encoded messages, (busy waiting) Wait until data is written!

  public: // Calls from the application side
    inline void putDataByte(const uint8_t c)  {   
        if(c == MARK) putCommand(STUFF);
        else putByte(c);
    }
    inline void putCommand(const uint16_t c) { // eg SYNC, BOM, EOM, STOP.... or ... normal data byte
        if(c == NODATA) return;
        putByte(msb(c)); // upcall to your virtual putByte
        putByte(lsb(c));
    }
    inline void sendMsg(const int len, const uint8_t* s) {
        putCommand(BOM);
        for(int i = 0; i < len; i++) putDataByte(*s++);
        putCommand(EOM);
    }
};


/*
 * To receive messages from UART/Radiolink and decode.
 * Case 1: Synchronous receiving. The calling function (upper SW layers) has the control
 * the receiving channel is always ready to provide data, e.g. from memory to memory,
 * or else busy waiting  until the receiving channel (UART/Radiolink) is ready to provide data.
 * The Upcall/Virtual function is on the side of the UART/Radiolink (lower SW layers)
 */

/*
 * Triggered from Application, Data Flow from UART/RadioLink to Application (get)
 */

class S3pReceiverSyncrhonous : S3pCode {
  private:
    // _________________________ UPCALL _________________ You shall override this method to read messages from the UART/Radiolink
    virtual uint8_t getByte() { return 0; } // override to read from UART/Radiolink encoded messages. (busy waiting) Wait until data is ready
    //___________________________________________________

  public:
    // Calls from the application side
    int getMsg(int maxLen, uint8_t* msg) {
        int len = -1;
        while(1) {
            uint8_t c = getByte();
            if(c != MARK && len >= 0 && len < maxLen) { msg[len++] = c; continue; }
            if(c == MARK) {
                uint16_t dualByte =  (uint16_t)(COMMAND | (uint16_t)getByte()); // we get most significant first now least significant
                switch(dualByte) {
                    case BOM:   len = 0;                   break;
                    case EOM:   if(len >= 0)  return len;  break;
                    case STUFF: if(len >= 0 && len < maxLen) msg[len++] = MARK; break;                     
                    default: executeCommand(dualByte);
                } // switch
            } // If command
        } // loop
    }
};
} // namespace RODOS

