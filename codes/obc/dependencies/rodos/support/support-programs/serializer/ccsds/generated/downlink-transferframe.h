

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct DownlinkTFHeader {
    static const uint16_t HEADER_SIZE = 6;
    uint8_t  version             ; //  2  bits  set to 0
    uint16_t spaceCraftId        ; // 10  bits  
    uint8_t  virtualChanId       ; //  3  bits  0: Real time, 1 history, 7 idle
    uint8_t  opControlFlag       ; //  1  bits  1 -> at the end of the TF is a 32-bit CLCW command status word
    uint8_t  masterChanFrameCnt  ; //  8  bits  
    uint8_t  virtualChanFrameCnt ; //  8  bits  
    uint8_t  secondHeaderFlag    ; //  1  bits  1-> there is a second. header (I recomend: NO secondary header)
    uint8_t  synchFlag           ; //  1  bits  set to 0
    uint8_t  packetOrderFlag     ; //  1  bits  set to 0
    uint8_t  segmentLenId        ; //  2  bits  set to 11
    uint16_t firstHeaderPtr      ; // 11  bits  normaly set to 0 , for idle frames set to 11111111110 (0x7fe)

    uint32_t serialize(uint8_t * const buf) const;
    uint32_t deserialize(uint8_t const * const buf);
};


inline uint32_t DownlinkTFHeader::serialize(uint8_t* const buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  2, version);
    setBitField(buf,   2, 10, spaceCraftId);
    setBitField(buf,  12,  3, virtualChanId);
    setBitField(buf,  15,  1, opControlFlag);
    buf[2]      =             masterChanFrameCnt;
    buf[3]      =             virtualChanFrameCnt;
    setBitField(buf,  32,  1, secondHeaderFlag);
    setBitField(buf,  33,  1, synchFlag);
    setBitField(buf,  34,  1, packetOrderFlag);
    setBitField(buf,  35,  2, segmentLenId);
    setBitField(buf,  37, 11, firstHeaderPtr);

    return 6;
}
inline uint32_t DownlinkTFHeader::deserialize(uint8_t const * const buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = static_cast<uint8_t>(getBitField(buf,   0,  2));
    spaceCraftId     = static_cast<uint16_t>(getBitField(buf,   2, 10));
    virtualChanId    = static_cast<uint8_t>(getBitField(buf,  12,  3));
    opControlFlag    = static_cast<uint8_t>(getBitField(buf,  15,  1));
    masterChanFrameCnt = buf[2];
    virtualChanFrameCnt = buf[3];
    secondHeaderFlag = static_cast<uint8_t>(getBitField(buf,  32,  1));
    synchFlag        = static_cast<uint8_t>(getBitField(buf,  33,  1));
    packetOrderFlag  = static_cast<uint8_t>(getBitField(buf,  34,  1));
    segmentLenId     = static_cast<uint8_t>(getBitField(buf,  35,  2));
    firstHeaderPtr   = static_cast<uint16_t>(getBitField(buf,  37, 11));

    return 6;
}
} // namespace end

