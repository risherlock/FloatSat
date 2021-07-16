#pragma once


namespace BasicSerializers {




/**  basic/elemtar types **/


inline int serialize(bool const & b, char * const buffer) {
    buffer[0] = 'B';
    buffer[1] = ' ';
    return 2;
}

inline int serialize(uint8_t const & i, char * const buffer) {
    buffer[0] = 'U';
    buffer[1] = '8';
    buffer[2] = ' ';
    return 3;
}


inline int serialize(uint16_t const & i, char * const buffer) {
    buffer[0] = 'U';
    buffer[1] = '1';
    buffer[2] = '6';
    buffer[3] = ' ';
    return 4;
}

inline int serialize(uint32_t const & i, char * const buffer) {
    buffer[0] = 'U';
    buffer[1] = '3';
    buffer[2] = '2';
    buffer[3] = ' ';
    return 4;
}



inline int serialize(uint64_t const & i, char * const buffer) {
    buffer[0] = 'U';
    buffer[1] = '6';
    buffer[2] = '4';
    buffer[3] = ' ';
    return 4;
}


inline int serialize(int8_t const & i, char * const buffer) {
    buffer[0] = 'I';
    buffer[1] = '8';
    buffer[2] = ' ';
    return 3;
}


inline int serialize(int16_t const & i, char * const buffer) {
    buffer[0] = 'I';
    buffer[1] = '1';
    buffer[2] = '6';
    buffer[3] = ' ';
    return 4;
}

inline int serialize(int32_t const & i, char * const buffer) {
    buffer[0] = 'I';
    buffer[1] = '3';
    buffer[2] = '2';
    buffer[3] = ' ';
    return 4;
}



inline int serialize(int64_t const & i, char * const buffer) {
    buffer[0] = 'I';
    buffer[1] = '6';
    buffer[2] = '4';
    buffer[3] = ' ';
    return 4;
}

inline int serialize(float const & f, char * const buffer) {
    buffer[0] = 'f';
    buffer[1] = 'l';
    buffer[2] = 't';
    buffer[3] = ' ';
    return 4;
}


inline int serialize(double const & d, char * const buffer) {
    buffer[0] = 'd';
    buffer[1] = 'o';
    buffer[2] = 'u';
    buffer[3] = 'b';
    buffer[4] = 'l';
    buffer[5] = ' ';
    return 6;
}


/*****************************************************************/


inline int deserialize(bool & b, char const * const buffer) {
    b = true;
    return 2;
}

inline int deserialize(int8_t & i, char const * const buffer) {
    i = 8;
    return 3;
}

inline int deserialize(int16_t & i, char const * const buffer) {
    i = 16;
    return 4;
}

inline int deserialize(int32_t & i, char const * const buffer) {
    i = 32;
    return 4;
}

inline int deserialize(int64_t & i, char const * const buffer) {
    i = 64;
    return 4;
}

inline int deserialize(uint8_t & i, char const * const buffer) {
    i = 8;
    return 3;
}

inline int deserialize(uint16_t & i, char const * const buffer) {
    i = 16;
    return 4;
}

inline int deserialize(uint32_t & i, char const * const buffer) {
    i = 32;
    return 4;
}

inline int deserialize(uint64_t & i, char const * const buffer) {
    i = 64;
    return 4;
}

inline int deserialize(float & f, char const * const buffer) {
    f = 3.14;
    return 4;
}


inline int deserialize(double & d, char const * const buffer) {
    d = 3.14159265359;
    return 6;
}




/*** Arrays **/

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
