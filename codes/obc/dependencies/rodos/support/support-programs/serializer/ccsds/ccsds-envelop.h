
#pragma once

#include "checksumes.h"
#include "generated/ccsds-headers.h"

#include "ccsds-mission-defs.h" // Defaults which shall be substituted by the mission

/* Max Downlink SP len:
 * TF_Len - TF_Header - SP_Header - TR_Trailer = TF_LEN -16 -6 -6  = TF_Len -28
 **/

namespace CCSDS {

//___________________________________________
struct UplinkTransferFrame  { unsigned char buf[CCSDS::UPLINK_TF_MAX_LEN]; };
struct UplinkSourcePacket   { unsigned char buf[CCSDS::UPLINK_SP_MAX_LEN]; };
struct DownlinkTransferFrame{ unsigned char buf[CCSDS::DOWNLINK_TF_LEN]; };
struct DownlinkSourcePacket { unsigned char buf[CCSDS::DOWNLINK_SP_MAX_LEN]; };


//___________________________________________

constexpr uint8_t ZERO_IN_SENSE_OF_UNDEF_OR_ERR = 0; ///< this is more clear than just writing 0

class CCSDSEnvelop {

protected:
    uint8_t buf[TF_MAX_LEN + 10]; // buf == Tranfer Frame. +10? we may try to read a SP beyond the end
    uint16_t indexOfCurrentUserData; ///< index = pos in buf

public:
    uint16_t lenOfCurrentTF;         ///< total len, set from receiver or when filling
    uint16_t lenOfCurrentSP;
    uint16_t lenOfCurrentUserData;   ///< of current SP, shall be set by user, who write the source packet
    uint16_t indexOfCurrentSP;       ///< index = pos in buf
    uint8_t*    userDataBuf;            ///< pointer to read/write data for correspondig source package

    //CCSDSEnvelop() { }
};


struct DownlinkEnvelop : CCSDSEnvelop {

    DownlinkTFHeader   tfHeader;
    DownlinkSPHeader   spHeader;     /// moving in TF
    DownlinkTFTrailer  tfTrailer;

    //_____________________________________ Same function nemas for Down & Uplink, but different implementation
    void initDefaultTFHeaderAndTrailer(uint16_t spaceCraftId); ///< for spacecraft
    void initDefaultSPHeader();           ///< for spacecraft
    void beginNewTF();			          ///< for spacecraft, request to write
    uint16_t beginNewSP();                    ///< for spacecraft, return max len of user data, 0-> TF is full, request to write
    void commitTF();                      ///< for spacecraft: serialize TF header and trailer and generate CRC
    void commitClcwToTF();                ///< for spacecraft: serialize trailer including CLCW and generate CRC
    void commitSP();                      ///< for spacecraft: if lenOfCurrentUserData is 0, the SP will be discarded
    void addIdleSP();                     ///< for spacecraft: the rest of a TF has to be an idle SP! will be called by commitSP()

    bool checkoutTF();                    ///< for groundstation: check CRC and deserialize TF header, returns ok
    bool checkoutNextSP();                ///< for groundstation

    DownlinkTransferFrame* getTfPtr() { return (DownlinkTransferFrame*) buf; }
    DownlinkSourcePacket*  getSpPtr() { return (DownlinkSourcePacket*) (buf+indexOfCurrentSP); }

};

//___________________________________________
struct UplinkEnvelop : CCSDSEnvelop {

    UplinkTFHeader   tfHeader;
    UplinkSPHeader   spHeader;
    UplinkTFTrailer  tfTrailer;

    //_____________________________________ Same function nemas for Down & Uplink, but different implementation
    void initDefaultTFHeaderAndTrailer(uint16_t spaceCraftId); ///< for groundstation
    void initDefaultSPHeader();           ///< for groundstation
    void beginNewTF();			          ///< for groundstation
    uint16_t  beginNewSP();                    ///< for groundstation  return max len of user data, 0-> TF is full
    void commitTF();                      ///< for groundstation: serialize TF header and generate CRC (for groundstation)
    void commitSP();                      ///< for groundstation: if lenOfCurrentUserData is 0, the SP will be discarded

    bool checkoutTF();                    ///< for spacecraft: check CRC and deserialize TF header, returns ok
    bool checkoutNextSP();                ///< for spacecraft

    UplinkTransferFrame* getTfPtr() { return (UplinkTransferFrame*) buf; }
    UplinkSourcePacket*  getSpPtr() { return (UplinkSourcePacket*) (buf+indexOfCurrentSP); }

    static uint32_t getLenOfCurrentUserData(UplinkSPHeader &spHeader) { return spHeader.length  + 1u - (spHeader.HEADER_SIZE - 6u); }
    static uint32_t getLenOfCurrentSP(UplinkSPHeader &spHeader)       { return getLenOfCurrentUserData(spHeader) + spHeader.HEADER_SIZE; }
};

//____________________________________________________
//extern DownlinkEnvelop downlinkEnvelop;
//extern UplinkEnvelop   uplinkEnvelop;
#ifndef NO_RODOS_NAMESPACE
extern RODOS::CRC      crcChecker;
#else
extern CRC             crcChecker;
#endif

} // namespace ccsds

