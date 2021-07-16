

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct DownlinkTFTrailer {
    static const uint16_t HEADER_SIZE = 6;
    uint8_t  control             ; //  1  bits  shall be 0
    uint8_t  version             ; //  2  bits  shall be 0
    uint8_t  status              ; //  3  bits  content not defined! recommend: 0
    uint8_t  commandOpProcedure  ; //  2  bits  COP in effect recommend 01
    uint8_t  virtualChanel       ; //  6  bits  
    uint8_t  spare1              ; //  2  bits  shall be 0
    uint8_t  noRF                ; //  1  bits  0 -> ok, 1-> no signal received
    uint8_t  noBitLock           ; //  1  bits  0 -> ok, 1-> signal to noisy
    uint8_t  lockOut             ; //  1  bits  0 -> ok, 1-> frame rejected
    uint8_t  wait                ; //  1  bits  
    uint8_t  retransmit          ; //  1  bits  
    uint8_t  farmBCnt            ; //  2  bits  current counter of framtes Type B
    uint8_t  spare2              ; //  1  bits  
    uint8_t  reportValue         ; //  8  bits  next expected frame sequence countrer of frames Type A 
    uint16_t crc                 ; // 16  bits  cyclic redundancy code in CCSDS called "FRAME ERROR CONTROL FIELD"

    uint32_t serialize(uint8_t * const buf) const;
    uint32_t deserialize(uint8_t const * const buf);
};


inline uint32_t DownlinkTFTrailer::serialize(uint8_t* const buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  1, control);
    setBitField(buf,   1,  2, version);
    setBitField(buf,   3,  3, status);
    setBitField(buf,   6,  2, commandOpProcedure);
    setBitField(buf,   8,  6, virtualChanel);
    setBitField(buf,  14,  2, spare1);
    setBitField(buf,  16,  1, noRF);
    setBitField(buf,  17,  1, noBitLock);
    setBitField(buf,  18,  1, lockOut);
    setBitField(buf,  19,  1, wait);
    setBitField(buf,  20,  1, retransmit);
    setBitField(buf,  21,  2, farmBCnt);
    setBitField(buf,  23,  1, spare2);
    buf[3]      =             reportValue;
    uint16_tToBigEndian(buf+4,  crc);

    return 6;
}
inline uint32_t DownlinkTFTrailer::deserialize(uint8_t const * const buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    control          = static_cast<uint8_t>(getBitField(buf,   0,  1));
    version          = static_cast<uint8_t>(getBitField(buf,   1,  2));
    status           = static_cast<uint8_t>(getBitField(buf,   3,  3));
    commandOpProcedure = static_cast<uint8_t>(getBitField(buf,   6,  2));
    virtualChanel    = static_cast<uint8_t>(getBitField(buf,   8,  6));
    spare1           = static_cast<uint8_t>(getBitField(buf,  14,  2));
    noRF             = static_cast<uint8_t>(getBitField(buf,  16,  1));
    noBitLock        = static_cast<uint8_t>(getBitField(buf,  17,  1));
    lockOut          = static_cast<uint8_t>(getBitField(buf,  18,  1));
    wait             = static_cast<uint8_t>(getBitField(buf,  19,  1));
    retransmit       = static_cast<uint8_t>(getBitField(buf,  20,  1));
    farmBCnt         = static_cast<uint8_t>(getBitField(buf,  21,  2));
    spare2           = static_cast<uint8_t>(getBitField(buf,  23,  1));
    reportValue      = buf[3];
    crc              = bigEndianToUint16_t(buf+4);

    return 6;
}
} // namespace end

