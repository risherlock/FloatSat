#pragma once

#include "rodos.h"

namespace BasicSerializers {

inline int serialize(bool const & b, char * const buffer)     { buffer[0] = b; return 1; }
inline int serialize(uint8_t const & i, char * const buffer)  { buffer[0] = i; return 1; }
inline int serialize(int8_t const & i, char * const buffer)   { buffer[0] = i; return 1; }

inline int serialize(uint16_t const & i, char * const buffer) { int16_tToBigEndian(buffer, i); return 2; }
inline int serialize(uint32_t const & i, char * const buffer) { int32_tToBigEndian(buffer, i); return 4; }
inline int serialize(uint64_t const & i, char * const buffer) { int64_tToBigEndian(buffer, i); return 8; }
inline int serialize(int16_t const & i, char * const buffer)  { int16_tToBigEndian(buffer, i); return 2; }
inline int serialize(int32_t const & i, char * const buffer)  { int32_tToBigEndian(buffer, i); return 4; }
inline int serialize(int64_t const & i, char * const buffer)  { int64_tToBigEndian(buffer, i); return 8; }
inline int serialize(float const & f, char * const buffer)    { floatToBigEndian(buffer, f);   return 4; }
inline int serialize(double const & d, char * const buffer)   { doubleToBigEndian(buffer, d);  return 8; }


/*****************************************************************/

inline int deserialize(bool & b, char const * const buffer)     { b = buffer[0]; return 1; }
inline int deserialize(int8_t & i, char const * const buffer)   { i = buffer[0]; return 1; }
inline int deserialize(uint8_t & i, char const * const buffer)  { i = buffer[0]; return 1; }

inline int deserialize(int16_t & i, char const * const buffer)  { i = bigEndianToInt16_t(buffer); return 2; }
inline int deserialize(int32_t & i, char const * const buffer)  { i = bigEndianToInt32_t(buffer); return 4; }
inline int deserialize(int64_t & i, char const * const buffer)  { i = bigEndianToInt64_t(buffer); return 8; }
inline int deserialize(uint16_t & i, char const * const buffer) { i = bigEndianToInt16_t(buffer); return 2; }
inline int deserialize(uint32_t & i, char const * const buffer) { i = bigEndianToInt32_t(buffer); return 4; }
inline int deserialize(uint64_t & i, char const * const buffer) { i = bigEndianToInt64_t(buffer); return 8; }
inline int deserialize(float & f, char const * const buffer)    { f = bigEndianToFloat(buffer);   return 4; }
inline int deserialize(double & d, char const * const buffer)   { d = bigEndianToDouble(buffer);  return 8; }


/********************************* Arrays **************************/

template<unsigned int N, typename T>
inline int serialize(T const (&array)[N], char * const buffer) {
  int size = 0;
  for (unsigned int i = 0; i < N; ++i) {
    size += serialize(array[i], buffer+size);
  }
  return size;
}


template<unsigned int N, typename T>
inline int deserialize(T (&array)[N], char const * const buffer) {
  int size = 0;
  for (unsigned int i = 0; i < N; ++i) {
    size += deserialize(array[i], buffer+size);
  }
  return size;
}



/** clases / structs **/

template<typename T>
inline int serialize(T const & obj, char * const buffer) {
    return obj.serialize(buffer);
}

template<typename T>
inline int deserialize(T & obj, char const * const buffer) {
    return obj.deserialize(buffer);
}



} // namespace
