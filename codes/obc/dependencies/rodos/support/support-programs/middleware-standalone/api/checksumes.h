/**
* @file checksumes.h
* @date 2008/06/17 10:46
* @author Sergio Montenegro
*
* @brief simple misc functions
*/

#pragma once

#include <cstddef>

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/** Computes a 16 bit checksum (len in bytes) adding bytes and rotating result */
uint16_t checkSum(const void *buf, size_t len) ;


/** computes a 16 bit crc in a non optimized way, CCSDS recommends 0 (some times 0xffff) as initial value **/
uint16_t computeCrc(const void* buf, size_t len, uint16_t initialValue);


/** An optimized crc generator.
  * first it generates a luck up table to speed up the crc computation
  * use only one in the system. more is not required.
  * CCSDS recommends 0 (but Warning: some times 0xffff) as initial value 
  */

class CRC {
    uint16_t lookUpTable[256];
public:
    CRC(); 
    uint16_t computeCRC(const void* buf, size_t len, uint16_t initialValue);
};


/** Delivers a 16 bit  hash value for a string.
 *  both bytes contain only printable characters
 */

uint16_t hash(const char* str);


/// Computes the checkSum of an array of 32-bit words, according to CCSDS algorithm.
/// Warning: len ist not bytes! it is num of 32-bit words
uint32_t checkSumXor32(const uint32_t *buf, size_t len);


#ifndef NO_RODOS_NAMESPACE
}
#endif

