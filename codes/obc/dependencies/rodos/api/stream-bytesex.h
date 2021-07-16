#pragma once

/** Bytesex Convertions
 *  byte string allwas in Big-endian (internet, ccsds, net, motorola) format
 *
 * Author    : Sergio Montenegro
 * Date      : 23.10.2008
 */

#include <cstdint>
#include <cstddef>

namespace RODOS {

/// Warning: Will be set in rodos-main(). never use beore main (eg never in static constructurs)
extern bool isHostBigEndian;

//______________________________________________________ toggle single variables

int16_t swap16(int16_t    sw_);
int32_t swap32(int32_t    lw_);
int64_t swap64(int64_t   llw_);
float   swapFloat(float   fw_);
double  swapDouble(double dw_);

short   shortConvertHost2Net(short sw_);       ///< DEPRECATED
long    longConvertHost2Net(long sw_);         ///< DEPRECATED
long long longlongConvertHost2Net(long long sw_);  ///< DEPRECATED

int16_t int16ConvertHost2Net(int16_t sw_);
int32_t int32ConvertHost2Net(int32_t lw_);
int64_t int64ConvertHost2Net(int64_t llw_);
float   floatConvertHost2Net(float   fw_);
double  doubleConvertHost2Net(double dw_);


//____________________________________________________________ Stream of bytes

/************ From a big-endian byte stream  to internal representation ****/

uint16_t  bigEndianToUint16_t (const void* byteStream);
uint32_t  bigEndianToUint32_t (const void* byteStream);
uint64_t  bigEndianToUint64_t (const void* byteStream);
int16_t   bigEndianToInt16_t  (const void* byteStream);
int32_t   bigEndianToInt32_t  (const void* byteStream);
int64_t   bigEndianToInt64_t  (const void* byteStream);
float  	  bigEndianToFloat    (const void* byteStream);
double    bigEndianToDouble   (const void* byteStream);

/************ From internal representation to big-endian  byte stream  ****/

void uint16_tToBigEndian (void* byteStream, uint16_t value);
void uint32_tToBigEndian (void* byteStream, uint32_t value);
void uint64_tToBigEndian (void* byteStream, uint64_t value);
void int16_tToBigEndian  (void* byteStream, int16_t  value);
void int32_tToBigEndian  (void* byteStream, int32_t  value);
void int64_tToBigEndian  (void* byteStream, int64_t  value);
void floatToBigEndian    (void* byteStream, float    value);
void doubleToBigEndian   (void* byteStream, double   value);


void setBitInByteStream (void *byteStream, int bitIndex, bool value);  //< bitIndex 0 .. N (very large), 0 = most-Sig Bit & Byte
int getBitFromByteStream(const void *byteStream, int bitIndex);        //< bitIndex 0 .. N (very large), 0 = most-Sig Bit & Byte

/// sets serval bits in a bigEndian bitfeld of max 16 bits (warning: ccsds: 0 = msb)
void setBitField(void* buffer, size_t bitPos, uint8_t numOfBits, uint32_t val);
/// gets severa bits from a bigEndia bitfled of max 16 bits (warning: ccsds: 0 = msb)
uint32_t getBitField(const void* buffer, size_t bitPos, uint8_t numOfBits);

/// gets the number of bits set
uint32_t getSetBits(const uint32_t &value);

}  // namespace


//_______________________________________________________________WARNING: not RODOS namepace to avoid ambigusities with "serialize"
namespace BasicSerializers {

inline uint32_t serialize(bool const & b, char * const buffer)     { buffer[0] = b ? 1 : 0; return 1; }
inline uint32_t serialize(uint8_t const & i, char * const buffer)  { buffer[0] = *reinterpret_cast<char const *>(&i); return 1; }
inline uint32_t serialize(int8_t const & i, char * const buffer)   { buffer[0] = *reinterpret_cast<char const *>(&i); return 1; }

inline uint32_t serialize(uint16_t const & i, char * const buffer) { RODOS::uint16_tToBigEndian(buffer, i); return 2; }
inline uint32_t serialize(uint32_t const & i, char * const buffer) { RODOS::uint32_tToBigEndian(buffer, i); return 4; }
inline uint32_t serialize(uint64_t const & i, char * const buffer) { RODOS::uint64_tToBigEndian(buffer, i); return 8; }
inline uint32_t serialize(int16_t const & i, char * const buffer)  { RODOS::int16_tToBigEndian(buffer, i); return 2; }
inline uint32_t serialize(int32_t const & i, char * const buffer)  { RODOS::int32_tToBigEndian(buffer, i); return 4; }
inline uint32_t serialize(int64_t const & i, char * const buffer)  { RODOS::int64_tToBigEndian(buffer, i); return 8; }
inline uint32_t serialize(float const & f, char * const buffer)    { RODOS::floatToBigEndian(buffer, f);   return 4; }
inline uint32_t serialize(double const & d, char * const buffer)   { RODOS::doubleToBigEndian(buffer, d);  return 8; }


/*****************************************************************/

inline uint32_t deserialize(bool & b, char const * const buffer)     { b = buffer[0]; return 1; }
inline uint32_t deserialize(int8_t & i, char const * const buffer)   { i = reinterpret_cast<int8_t const *>(buffer)[0]; return 1; }
inline uint32_t deserialize(uint8_t & i, char const * const buffer)  { i = reinterpret_cast<uint8_t const *>(buffer)[0]; return 1; }

inline uint32_t deserialize(int16_t & i, char const * const buffer)  { i = RODOS::bigEndianToInt16_t(buffer); return 2; }
inline uint32_t deserialize(int32_t & i, char const * const buffer)  { i = RODOS::bigEndianToInt32_t(buffer); return 4; }
inline uint32_t deserialize(int64_t & i, char const * const buffer)  { i = RODOS::bigEndianToInt64_t(buffer); return 8; }
inline uint32_t deserialize(uint16_t & i, char const * const buffer) { i = RODOS::bigEndianToUint16_t(buffer); return 2; }
inline uint32_t deserialize(uint32_t & i, char const * const buffer) { i = RODOS::bigEndianToUint32_t(buffer); return 4; }
inline uint32_t deserialize(uint64_t & i, char const * const buffer) { i = RODOS::bigEndianToUint64_t(buffer); return 8; }
inline uint32_t deserialize(float & f, char const * const buffer)    { f = RODOS::bigEndianToFloat(buffer);   return 4; }
inline uint32_t deserialize(double & d, char const * const buffer)   { d = RODOS::bigEndianToDouble(buffer);  return 8; }


/********************************* Arrays **************************/

template<unsigned int N, typename T>
inline uint32_t serialize(T const (&array)[N], char * const buffer) {
    uint32_t size = 0;
    for (unsigned int i = 0; i < N; ++i) {
        size += serialize(array[i], buffer+size);
    }
    return size;
}

template<unsigned int N, typename T>
inline uint32_t deserialize(T (&array)[N], char const * const buffer) {
    uint32_t size = 0;
    for (unsigned int i = 0; i < N; ++i) {
        size += deserialize(array[i], buffer+size);
    }
    return size;
}

/** clases / structs **/

template<typename T>
inline uint32_t serialize(T const & obj, char * const buffer) {
    return obj.serialize(buffer);
}

template<typename T>
inline uint32_t deserialize(T & obj, char const * const buffer) {
    return obj.deserialize(buffer);
}

} // namespace
