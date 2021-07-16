#include <stdint.h>
#include "stream-bytesex.h"
#include "misc-rodos-funcs.h"

/** Bytesex Convertions
 *  byte string allwas in Big-endian (internet, ccsds, net, motorola) format
 *
 * Author    : Sergio Montenegro
 * Date      : 23.10.2008
 */

/************ From a big-endian byte stram  to internal representation ****/

namespace RODOS {

/// exchange values of char variables (a) and (b)
inline void swapChars(char& a, char& b) {
    const char tmp = a;
    a              = b;
    b              = tmp;
}

int16_t swap16(int16_t sw_);
int32_t swap32(int32_t sw_);
int64_t swap64(int64_t sw_);
float   swapFloat(float   sw_); 
double  swapDouble(double  sw_);
int16_t int16ConvertHost2Net(int16_t sw_); 
int32_t int32ConvertHost2Net(int32_t sw_);
int64_t int64ConvertHost2Net(int64_t sw_); 
float   floatConvertHost2Net(float   sw_); 
double  doubleConvertHost2Net(double sw_); 



int16_t swap16(int16_t sw_) {
    int16_t sw = sw_;
    char *c = (char*)&sw;
    swapChars(c[0], c[1]);
    return sw;
}

int32_t swap32(int32_t lw_) {
    int32_t lw = lw_;
    char *c = (char*)&lw;
    swapChars(c[0], c[3]);
    swapChars(c[1], c[2]);
    return lw;
}

float swapFloat(float fw_) {
    float fw = fw_;
    char *c = (char*)&fw;
    swapChars(c[0], c[3]);
    swapChars(c[1], c[2]);
    return fw;
}


int64_t swap64(int64_t llw_) {
    int64_t  llw = llw_;
    char *c = (char*)&llw;
    swapChars(c[0], c[7]);
    swapChars(c[1], c[6]);
    swapChars(c[2], c[5]);
    swapChars(c[3], c[4]);
    return llw;
}

double swapDouble(double dw_) {
    double  dw = dw_;
    char *c = (char*)&dw;
    swapChars(c[0], c[7]);
    swapChars(c[1], c[6]);
    swapChars(c[2], c[5]);
    swapChars(c[3], c[4]);
    return dw;
}

int16_t int16ConvertHost2Net(int16_t sw) {
    if(isHostBigEndian) return sw;
    return swap16(sw); 
}

int32_t int32ConvertHost2Net(int32_t lw) {
    if(isHostBigEndian) return lw;
    return swap32(lw); 
}

int64_t int64ConvertHost2Net(int64_t llw) {
    if(isHostBigEndian) return llw;
    return swap64(llw); 
}

float floatConvertHost2Net(float fw) {
    if(isHostBigEndian) return fw;
    return swapFloat(fw); 
}

double doubleConvertHost2Net(double dw) {
    if(isHostBigEndian) return dw;
    return swapDouble(dw); 
}


/// DEPRECATED... 
int16_t shortConvertHost2Net(int16_t sw) { if(isHostBigEndian) return sw; return swap16(sw); } 
/// DEPRECATED... 
int32_t  longConvertHost2Net(int32_t lw) { if(isHostBigEndian) return lw; return swap32(lw); }
/// DEPRECATED... 
int64_t longlongConvertHost2Net(int64_t llw) { if(isHostBigEndian) return llw; return swap64(llw); } 


//__________________________________________________________________________________________________________

uint16_t bigEndianToUint16_t(const void* buff) {
    const uint8_t* byteStream = (const uint8_t*)buff;
    if (byteStream == 0) return 0;
    return static_cast<uint16_t>(
              (((uint16_t)(byteStream[0])) << 8)
            | ((uint16_t)(byteStream[1]))
        );
}

uint32_t bigEndianToUint32_t(const void* buff) {
    const uint8_t* byteStream = (const uint8_t*)buff;
    if (byteStream == 0) return 0;
    return static_cast<uint32_t>(
              (((uint32_t)(byteStream[0])) << 24)
            | (((uint32_t)(byteStream[1])) << 16)
            | (((uint32_t)(byteStream[2])) << 8)
            | ((uint32_t)(byteStream[3]))
        );
}


uint64_t bigEndianToUint64_t(const void* buff) {
    const uint8_t* byteStream = (const uint8_t*)buff;
    if (byteStream == 0) return 0;
    return static_cast<uint64_t>(
              (((uint64_t)(byteStream[0])) << 56)
            | (((uint64_t)(byteStream[1])) << 48)
            | (((uint64_t)(byteStream[2])) << 40)
            | (((uint64_t)(byteStream[3])) << 32)
            | (((uint64_t)(byteStream[4])) << 24)
            | (((uint64_t)(byteStream[5])) << 16)
            | (((uint64_t)(byteStream[6])) <<  8)
            |  ((uint64_t)(byteStream[7]))
        );
}


float bigEndianToFloat(const void* buff) {
    const uint8_t* byteStream = (const uint8_t*)buff;
    union {
        float value;
        uint32_t lvalue;
    } value_union;
    value_union.lvalue = bigEndianToUint32_t(byteStream);
    return value_union.value;
}

double bigEndianToDouble(const void* buff) {
    union {
        double value;
        uint64_t llvalue;
    } value_union;
    value_union.llvalue = bigEndianToUint64_t(buff);
    return value_union.value;
}

/************ From internal representation to big-endian  byte stram  ****/

void uint16_tToBigEndian(void* buff, uint16_t value) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return;
    byteStream[0] = (uint8_t) ((value >> 8) & 0xFF);
    byteStream[1] = (uint8_t) ((value >> 0) & 0xFF);
}

void uint32_tToBigEndian(void* buff, uint32_t value) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return;
    byteStream[0] = (uint8_t) ((value >> 24) & 0xFF);
    byteStream[1] = (uint8_t) ((value >> 16) & 0xFF);
    byteStream[2] = (uint8_t) ((value >>  8) & 0xFF);
    byteStream[3] = (uint8_t) ((value >>  0) & 0xFF);
}


void uint64_tToBigEndian(void* buff, uint64_t value) {
    uint8_t* byteStream = (uint8_t*)buff;
    if (byteStream == 0) return;
    byteStream[0] = (uint8_t) ((value >> 56) & 0xFF);
    byteStream[1] = (uint8_t) ((value >> 48) & 0xFF);
    byteStream[2] = (uint8_t) ((value >> 40) & 0xFF);
    byteStream[3] = (uint8_t) ((value >> 32) & 0xFF);
    byteStream[4] = (uint8_t) ((value >> 24) & 0xFF);
    byteStream[5] = (uint8_t) ((value >> 16) & 0xFF);
    byteStream[6] = (uint8_t) ((value >>  8) & 0xFF);
    byteStream[7] = (uint8_t) ((value >>  0) & 0xFF);
}

int16_t bigEndianToInt16_t (const void* byteStream) { return static_cast<int16_t>( bigEndianToUint16_t(byteStream) ); }
int32_t bigEndianToInt32_t (const void* byteStream) { return static_cast<int32_t>( bigEndianToUint32_t(byteStream) ); }
int64_t bigEndianToInt64_t (const void* byteStream) { return static_cast<int64_t>( bigEndianToUint64_t(byteStream) ); }
void    int16_tToBigEndian (void* byteStream, int16_t value) { uint16_tToBigEndian(byteStream, static_cast<uint16_t>(value)); }
void    int32_tToBigEndian (void* byteStream, int32_t value) { uint32_tToBigEndian(byteStream, static_cast<uint32_t>(value)); }
void    int64_tToBigEndian (void* byteStream, int64_t value) { uint64_tToBigEndian(byteStream, static_cast<uint64_t>(value)); }

void floatToBigEndian(void* buff, float value_) {
    uint8_t* byteStream = (uint8_t*)buff;
    union {
        float value;
        uint32_t lvalue;
    } value_union;
    value_union.value = value_;
    uint32_tToBigEndian(byteStream, value_union.lvalue);
}

void doubleToBigEndian   (void* buff, double value_) {
    uint8_t* byteStream = (uint8_t*)buff;
    union {
        double value;
        uint64_t llvalue;
    } value_union;
    value_union.value = value_;
    uint64_tToBigEndian(byteStream, value_union.llvalue);
}


/*************************************************/

void setBitInByteStream (void *byteStream, int bitIndex, bool value) {
    int byteIndex = bitIndex / 8;
    int bitPos =    bitIndex % 8;
    unsigned char* bytes = (unsigned char*)byteStream;

    if (value) {
        bytes[byteIndex] = static_cast<unsigned char>(bytes[byteIndex] | (0x1 << (7 - bitPos)));
    } else {
        bytes[byteIndex] = static_cast<unsigned char>(bytes[byteIndex] & ~(0x1 << (7 - bitPos)));
    }
}


int getBitFromByteStream(const void *byteStream, int bitIndex) {

    const uint8_t* bytes = (const uint8_t*)byteStream;

    unsigned char selectedByte = bytes[bitIndex/8];
    return ((selectedByte) >> (7 - (bitIndex % 8))) & 0x01;
}


/***********************************************************/


/** Sets a bit field form 0 to 16 bits for CCSDS bitd fields 
  * Warning: CCSDS -> Bit 0 = most significant bit!
  **/

void setBitField(void* buffer, size_t bitPos, uint8_t numOfBits, uint32_t val) {
    unsigned char* buf = (unsigned char*) buffer;
    size_t byteIndex = bitPos / 8;
    bitPos        = bitPos % 8;
    uint8_t shifts    = static_cast<uint8_t>(24 - (bitPos + numOfBits));
    uint32_t mask     = uint32_tOnes(numOfBits) << shifts;

    val = val << shifts;

    // get the word as big-endian (CPU independent)
    uint32_t word    = static_cast<uint32_t>(buf[byteIndex]) << 16u;
	if(bitPos+numOfBits > 8 ) word |=  static_cast<uint32_t>(buf[byteIndex+1]) << 8u; //Do not read byte not required to avoid reading beyond the buffer
	if(bitPos+numOfBits > 16) word |=  buf[byteIndex+2];

    word  &= ~ mask;        // Clear bit field
    word  |=  mask & val;   // set the correspondig bits

    // store the word as big-endian (CPU independent)
    buf[byteIndex]                             = (word >> 16) & 0xff; // Most Significant first
    if(bitPos+numOfBits > 8 ) buf[byteIndex+1] = (word >> 8 ) & 0xff;
    if(bitPos+numOfBits > 16) buf[byteIndex+2] =  word        & 0xff;
}


/** gets a bit field form 0 to 16 bits for CCSDS bitd fields 
  * Warning: CCSDS -> Bit 0 = most significant bit!
  **/

uint32_t getBitField(const void* buffer, size_t bitPos, uint8_t numOfBits) {
    const uint8_t* buf = (const uint8_t*) buffer;
    size_t byteIndex = bitPos / 8;
    bitPos        = bitPos % 8;
    uint8_t shifts    = static_cast<uint8_t>(24 - (bitPos + numOfBits));
    uint32_t mask     = uint32_tOnes(numOfBits);  // so many bits set to 1 from leastst significant


    // get the word as big-endian (CPU independent)
    uint32_t word    = static_cast<uint32_t>(buf[byteIndex]) << 16u;
    if(bitPos+numOfBits > 8 ) word |=  static_cast<uint32_t>(buf[byteIndex+1]) << 8u; //Do not read byte not required to avoid reading beyond the buffer
    if(bitPos+numOfBits > 16) word |=  buf[byteIndex+2];

    return  (word >> shifts) &  mask;
}

// counts number of bits set to 1
uint32_t getSetBits(const uint32_t &value) {

    // reference: Hacker's Delight, p. 66, Figure 5-2
    uint32_t x = value;
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

}


