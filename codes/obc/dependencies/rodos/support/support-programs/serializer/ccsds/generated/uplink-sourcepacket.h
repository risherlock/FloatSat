

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkSPHeader {
    static const uint16_t HEADER_SIZE = 10;
    uint8_t  version             ; //  3  bits  set to 0
    uint8_t  type                ; //  1  bits  1 -> telecommand
    uint8_t  secondaryHeaderFlag ; //  1  bits  set to 1 (We have a PUS header)
    uint16_t applicationId       ; // 11  bits  
    uint8_t  sequenceFlags       ; //  2  bits  set 3 for standalone
    uint16_t sequenceCounter     ; // 14  bits  
    uint16_t length              ; // 16  bits  real_len - 1
    uint8_t  pusSecondaryHeaderFlag; //  1  bits  set to 0
    uint8_t  pusVersion          ; //  3  bits  set to 1
    uint8_t  ackType             ; //  4  bits  4x Bool: (completion, execution, start, acceptance)
    uint8_t  serviceType         ; //  8  bits  
    uint8_t  serviceSubtype      ; //  8  bits  
    uint8_t  sourceID            ; //  8  bits  set to  0

    uint32_t serialize(uint8_t * const buf) const;
    uint32_t deserialize(uint8_t const * const buf);
};


inline uint32_t UplinkSPHeader::serialize(uint8_t* const buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  3, version);
    setBitField(buf,   3,  1, type);
    setBitField(buf,   4,  1, secondaryHeaderFlag);
    setBitField(buf,   5, 11, applicationId);
    setBitField(buf,  16,  2, sequenceFlags);
    setBitField(buf,  18, 14, sequenceCounter);
    uint16_tToBigEndian(buf+4,  length);
    setBitField(buf,  48,  1, pusSecondaryHeaderFlag);
    setBitField(buf,  49,  3, pusVersion);
    setBitField(buf,  52,  4, ackType);
    buf[7]      =             serviceType;
    buf[8]      =             serviceSubtype;
    buf[9]      =             sourceID;

    return 10;
}
inline uint32_t UplinkSPHeader::deserialize(uint8_t const * const buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = static_cast<uint8_t>(getBitField(buf,   0,  3));
    type             = static_cast<uint8_t>(getBitField(buf,   3,  1));
    secondaryHeaderFlag = static_cast<uint8_t>(getBitField(buf,   4,  1));
    applicationId    = static_cast<uint16_t>(getBitField(buf,   5, 11));
    sequenceFlags    = static_cast<uint8_t>(getBitField(buf,  16,  2));
    sequenceCounter  = static_cast<uint16_t>(getBitField(buf,  18, 14));
    length           = bigEndianToUint16_t(buf+4);
    pusSecondaryHeaderFlag = static_cast<uint8_t>(getBitField(buf,  48,  1));
    pusVersion       = static_cast<uint8_t>(getBitField(buf,  49,  3));
    ackType          = static_cast<uint8_t>(getBitField(buf,  52,  4));
    serviceType      = buf[7];
    serviceSubtype   = buf[8];
    sourceID         = buf[9];

    return 10;
}
} // namespace end

