

//--- generated file, DO NOT modify
//--- see bitFieldsSerializer
#pragma once


namespace CCSDS {
struct UplinkTFHeader {
    static const uint16_t HEADER_SIZE = 6;
    uint8_t  version             ; //  2  bits  set to 0
    uint8_t  bypassFlag          ; //  1  bits  0->A: normal (check SeqNr),  1->B: Bypass, accept all cmds
    uint8_t  controllCommandFlag ; //  1  bits  0->D (data upload protocoll) 1 -> C (Command protocoll)
    uint8_t  spare1              ; //  2  bits  set to 0
    uint16_t spacecraftID        ; // 10  bits  
    uint8_t  virtualChannelID    ; //  6  bits  
    uint16_t frameLength         ; // 10  bits  frameLength = real_len - 1
    uint8_t  frameSequenceNr     ; //  8  bits  
    uint8_t  sequenceFlags       ; //  2  bits  set to 0x3 
    uint8_t  multiplexAceessPoint; //  6  bits  set to 0x3 , eg to select target computer

    uint32_t serialize(uint8_t * const buf) const;
    uint32_t deserialize(uint8_t const * const buf);
};


inline uint32_t UplinkTFHeader::serialize(uint8_t* const buf) const {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    setBitField(buf,   0,  2, version);
    setBitField(buf,   2,  1, bypassFlag);
    setBitField(buf,   3,  1, controllCommandFlag);
    setBitField(buf,   4,  2, spare1);
    setBitField(buf,   6, 10, spacecraftID);
    setBitField(buf,  16,  6, virtualChannelID);
    setBitField(buf,  22, 10, frameLength);
    buf[4]      =             frameSequenceNr;
    setBitField(buf,  40,  2, sequenceFlags);
    setBitField(buf,  42,  6, multiplexAceessPoint);

    return 6;
}
inline uint32_t UplinkTFHeader::deserialize(uint8_t const * const buf) {
    #ifndef NO_RODOS_NAMESPACE
    using namespace RODOS;
    #endif

    version          = static_cast<uint8_t>(getBitField(buf,   0,  2));
    bypassFlag       = static_cast<uint8_t>(getBitField(buf,   2,  1));
    controllCommandFlag = static_cast<uint8_t>(getBitField(buf,   3,  1));
    spare1           = static_cast<uint8_t>(getBitField(buf,   4,  2));
    spacecraftID     = static_cast<uint16_t>(getBitField(buf,   6, 10));
    virtualChannelID = static_cast<uint8_t>(getBitField(buf,  16,  6));
    frameLength      = static_cast<uint16_t>(getBitField(buf,  22, 10));
    frameSequenceNr  = buf[4];
    sequenceFlags    = static_cast<uint8_t>(getBitField(buf,  40,  2));
    multiplexAceessPoint = static_cast<uint8_t>(getBitField(buf,  42,  6));

    return 6;
}
} // namespace end

