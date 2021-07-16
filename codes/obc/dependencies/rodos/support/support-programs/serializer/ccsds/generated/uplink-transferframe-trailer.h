

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkTFTrailer {
    static const uint16_t HEADER_SIZE = 2;
    uint16_t crc                 ; // 16  bits  cyclic redundancy code in CCSDS called "FRAME ERROR CONTROL FIELD"

    uint32_t serialize(uint8_t * const buf) const;
    uint32_t deserialize(uint8_t const * const buf);
};


inline uint32_t UplinkTFTrailer::serialize(uint8_t* const buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    uint16_tToBigEndian(buf+0,  crc);

    return 2;
}
inline uint32_t UplinkTFTrailer::deserialize(uint8_t const * const buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    crc              = bigEndianToUint16_t(buf+0);

    return 2;
}
} // namespace end

