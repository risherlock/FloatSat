#pragma once

/*
 * S3P Message framing, 
 * @author: Sergio Montenegro, Sept 2019, 2021
 */

/** please read s3p-encoder-in-byte stream.pdf **/

#include <stdint.h>

namespace RODOS {

/** Framing of messages in a byte stream : BOM, data EOM  all Bigeindiag, most significant byte first**/
class S3pCode {
  public:
    static const uint16_t NEVER_USE= 0xFEFE; // Never, Never  send (MARK,MARK)
    static const uint16_t BOM      = 0xFE02; // Begin of Message
    static const uint16_t EOM      = 0xFE03; // End of Message
    static const uint16_t STUFF    = 0xFE7E; // Stuffing for data whit same bit pattern as commands
    static const uint16_t STOP     = 0xFE13;
    static const uint16_t CONTINUE = 0xFE11;
    static const uint16_t SYNC     = 0xFE16; // command for time synchronisation e.g., One Pulse Per Second
    static const uint16_t NODATA   = 0xFEAA; // Filler, nothing to send. Not used for UARTS, only for non stoppable lines
    static const uint16_t COMMAND  = 0xFE00; // be aware: (COMMAND >> 8) == MARK
    static const uint16_t MARK     = 0x00FE; // WARNING: If databyte, it has to be encoded as STUFF
    static const uint16_t DATABYTE = 0x00FF; // 0x00XX means a single byte XX as data.

    static inline bool     isMark(const uint16_t c)      { return (c & 0x00ff) == MARK; }
    static inline bool     isCommand(const uint16_t c)   { return (c & 0xff00) == COMMAND; }
    static inline bool     hasMark(const uint16_t c)     { return isCommand(c) || isMark(c); }
    static inline bool     isDataByte(const uint16_t c)  { return (c & 0xff00) == 0; }
    static inline uint8_t  dataByte(const uint16_t c)    { return (uint8_t)(c & 0x00ff); }

    static inline uint8_t  msb(const uint16_t c) { return (uint8_t)(c >> 8); }   // Most Significant Byte.  Bigendian: MSB First
    static inline uint8_t  lsb(const uint16_t c) { return (uint8_t)(c & 0xff); } // Least Significant Byte. Bigendian: MSB First
    static inline uint16_t compose(const uint8_t msb, const uint8_t lsb) { return (uint16_t)((uint16_t)msb << 8) | (uint16_t)lsb; }

    virtual void executeCommand([[gnu::unused]] uint16_t command) {}   // override execute commands other than BOM, EOM and STUFF
};

} // namespace
