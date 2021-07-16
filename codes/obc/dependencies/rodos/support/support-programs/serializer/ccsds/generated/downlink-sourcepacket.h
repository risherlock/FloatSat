

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct DownlinkSPHeader {
    static const uint16_t HEADER_SIZE = 16;
    uint8_t  version             ; //  3  bits  just ignore! set to 0
    uint8_t  typeId              ; //  1  bits  shall be 0!
    uint8_t  secHeaderFlag       ; //  1  bits  1 -> secondary header follows
    uint16_t applicationId       ; // 11  bits  normaly called APID, for idle set to 11111111111 = 0x7ff
    uint8_t  groupingFlags       ; //  2  bits  00: cont packet, 01: first one, 10: last one, 11: no groupping (11 Recommended!)
    uint16_t sourceSeqCnt        ; // 14  bits  please count only not idle packeges
    uint16_t dataPackLen         ; // 16  bits  Warning! = len(secondaryheader + userdata) - 1 , in our case here 10 + userdatalen - 1
    uint8_t  secondaryFlag       ; //  1  bits  shall be 0
    uint8_t  pusVersion          ; //  3  bits  ignore! set to 1
    uint8_t  spare               ; //  4  bits  shall be 0
    uint8_t  service             ; //  8  bits  3  = normal bus telemetry
    uint8_t  subservice          ; //  8  bits  25 = Automatic HKD
    uint8_t  destination         ; //  8  bits  0 for Groundstation (normal case) ; WARNING: Some expect here source (not dest)
    uint32_t timeStampSeconds    ; // 32  bits  UTC in seconds since 1.1.2000  0:00, set by Housekeeper
    uint16_t timeStampFraction   ; // 16  bits  (1/2^16) of the second

    uint32_t serialize(uint8_t * const buf) const;
    uint32_t deserialize(uint8_t const * const buf);
};


inline uint32_t DownlinkSPHeader::serialize(uint8_t* const buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  3, version);
    setBitField(buf,   3,  1, typeId);
    setBitField(buf,   4,  1, secHeaderFlag);
    setBitField(buf,   5, 11, applicationId);
    setBitField(buf,  16,  2, groupingFlags);
    setBitField(buf,  18, 14, sourceSeqCnt);
    uint16_tToBigEndian(buf+4,  dataPackLen);
    setBitField(buf,  48,  1, secondaryFlag);
    setBitField(buf,  49,  3, pusVersion);
    setBitField(buf,  52,  4, spare);
    buf[7]      =             service;
    buf[8]      =             subservice;
    buf[9]      =             destination;
    uint32_tToBigEndian(buf+10,  timeStampSeconds);
    uint16_tToBigEndian(buf+14,  timeStampFraction);

    return 16;
}
inline uint32_t DownlinkSPHeader::deserialize(uint8_t const * const buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = static_cast<uint8_t>(getBitField(buf,   0,  3));
    typeId           = static_cast<uint8_t>(getBitField(buf,   3,  1));
    secHeaderFlag    = static_cast<uint8_t>(getBitField(buf,   4,  1));
    applicationId    = static_cast<uint16_t>(getBitField(buf,   5, 11));
    groupingFlags    = static_cast<uint8_t>(getBitField(buf,  16,  2));
    sourceSeqCnt     = static_cast<uint16_t>(getBitField(buf,  18, 14));
    dataPackLen      = bigEndianToUint16_t(buf+4);
    secondaryFlag    = static_cast<uint8_t>(getBitField(buf,  48,  1));
    pusVersion       = static_cast<uint8_t>(getBitField(buf,  49,  3));
    spare            = static_cast<uint8_t>(getBitField(buf,  52,  4));
    service          = buf[7];
    subservice       = buf[8];
    destination      = buf[9];
    timeStampSeconds = bigEndianToUint32_t(buf+10);
    timeStampFraction = bigEndianToUint16_t(buf+14);

    return 16;
}
} // namespace end

