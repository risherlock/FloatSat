
/**
* @file checksumes.cc
* @date 2008/04/22 17:08
* @author Lutz Dittrich, Sergio Montenegro
*
* @brief small support functions
*
*/
#include <string_pico.h>
//#include "hw_specific.h"
//#include "rodos.h"
#include "checksumes.h"

namespace RODOS {

/****************************************************/


inline void rotateRight(uint16_t& c) {
    if (c & 0x01) {
        c = static_cast<uint16_t>((c >> 1) | static_cast<uint16_t>(0x8000));
    } else {
        c = static_cast<uint16_t>(c >> 1);
    }
}

/** Computes a 16-bit checksum (len in bytes) adding bytes and rotating result */

uint16_t checkSum(const void *buf, size_t len) {

	uint16_t checksum = 0; /* The checksum mod 2^16. */
	size_t cnt;
    const uint8_t* data = static_cast<const uint8_t*>(buf);

    for(cnt = 0; cnt < len; cnt++) {
		rotateRight(checksum);
		checksum = static_cast<uint16_t>(checksum + data[cnt]);
	}
	return checksum;

}



/** computes a 16-bit crc in a non optimized way **/

uint16_t computeCrc(const void* buf, size_t len, uint16_t initialValue) {

    uint16_t currentValue = initialValue;
    const uint8_t* data = static_cast<const uint8_t*>(buf);

    for(size_t charCnt = 0; charCnt < len; charCnt++) {
        uint8_t curChar = data[charCnt];
        for(int bitCnt = 0; bitCnt < 8; bitCnt++) {
            if((curChar & 0x80u) ^ ((currentValue & 0x8000u) >> 8u)) {
                currentValue = static_cast<uint16_t>((static_cast<uint32_t>(currentValue << 1u)  ^ 0x1021u)); // Standard Polynom for CCSDS
            } else {
                currentValue = static_cast<uint16_t>(currentValue << 1u);
            }
            curChar = static_cast<uint8_t>(curChar << 1u);
        }
    }
    return currentValue;

}


/** An optimized crc generator.
  * first it generates a luck up table to speed up the crc computation
  */

CRC::CRC() {
    for (int i=0; i < 256; i++) {
        uint16_t tmp=0;
        if ((i & 1) != 0)   tmp=tmp ^ 0x1021;
        if ((i & 2) != 0)   tmp=tmp ^ 0x2042;
        if ((i & 4) != 0)   tmp=tmp ^ 0x4084;
        if ((i & 8) != 0)   tmp=tmp ^ 0x8108;
        if ((i & 16) != 0)  tmp=tmp ^ 0x1231;
        if ((i & 32) != 0)  tmp=tmp ^ 0x2462;
        if ((i & 64) != 0)  tmp=tmp ^ 0x48C4;
        if ((i & 128) != 0) tmp=tmp ^ 0x9188;
        lookUpTable [i] = tmp;
    }
}

uint16_t CRC::computeCRC(const void* buf, size_t len, uint16_t initialValue) {
    uint16_t currentValue = initialValue;
    const uint8_t* data = static_cast<const uint8_t*>(buf);

    for(size_t i = 0; i < len; i++) {
        currentValue = static_cast<uint16_t>(
			(static_cast<uint32_t>(currentValue << 8u) & 0xFF00u)
			^ lookUpTable [(((currentValue >> 8)^ data[i]) & 0x00FF)]);
    }
    return currentValue;
}


/** Delivers a 16-bit hash value for a string.
 *  both bytes contain only printable characters
 */

uint16_t hash(const char* str) {
	uint16_t crc = computeCrc(str, strlen(str), 0xffffu);

	/** To make only printable characters, else it were a normal crc value **/

		uint8_t a = static_cast<uint8_t>((crc >> 8) & 0xff);
		uint8_t b = static_cast<uint8_t>(crc & 0xff);

		uint8_t range = 0x7e - 0x20; // Printable ascii chars
		if(a <= 0x20 || a >= 0x7e) a = static_cast<uint8_t>((a % range) + 0x20);
		if(b <= 0x20 || b >= 0x7e) b = static_cast<uint8_t>((b % range) + 0x20);

	/******/

	return static_cast<uint16_t>((a << 8u) | b);
}



/** Computes a checksum (len in 32-bit words) */
uint32_t checkSumXor32(const uint32_t *buf, size_t len) {

    uint32_t cksum = 0;
    for (size_t i = 0; i < len; ++i) {
        cksum ^= buf[i];
    }
    return cksum;
}


} // namespace

