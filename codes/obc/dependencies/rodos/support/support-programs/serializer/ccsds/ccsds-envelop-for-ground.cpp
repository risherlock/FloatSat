

#include "rodos.h"
#include "ccsds-envelop.h"

// envelop for Groundsegment
//     Allmost (but only allmost) identic to the spacecraft code

namespace CCSDS {

//CRC             crcChecker;
Application     applicationGround("CCSDS-groundsegment-envelop");

/*************************** To Write (Send) Data **********************/

//______________________________________________________ init Uplinks with default values (Code for groundstation)

void UplinkEnvelop::initDefaultTFHeaderAndTrailer(uint16_t spaceCraftId) {

    memset(&tfHeader,  '\0', sizeof(tfHeader));
    memset(&tfTrailer, '\0', sizeof(tfTrailer));

    tfHeader.bypassFlag            = 1; //    0->A: normal (check SeqNr),  1->B: Bypass, accept all cmds
    tfHeader.controllCommandFlag   = 1; //    0->D (data upload protocoll) 1 -> C (Command protocoll)
    tfHeader.spacecraftID          = spaceCraftId;
    tfHeader.frameLength           = tfHeader.HEADER_SIZE + tfTrailer.HEADER_SIZE - 1u;    //  frameLength = real_len - 1; min: TF_header+trailer-1 = 6+2-1

    // segment header
    tfHeader.sequenceFlags         = 3;
    tfHeader.multiplexAceessPoint  = 3;
}

void UplinkEnvelop::initDefaultSPHeader() {


    memset(&spHeader, '\0', sizeof(spHeader));
    spHeader.type                 = 1;  //  1 -> telecommand
    spHeader.secondaryHeaderFlag  = 1;  //  set to 1 (We have a PUS header)
    spHeader.sequenceFlags        = 3;
    spHeader.length               = spHeader.HEADER_SIZE - 1;  // len-1 ; min header -1 == 10-1

    //  Secondary (PUS) Header, secondary header
    spHeader.pusVersion           = 1;
    spHeader.ackType              = 1;  //    only acceptance: 4x bits: (completion, execution, start, acceptance)
}

//_______________________________________________________________ commits for uplink (code for ground station)

void UplinkEnvelop::beginNewTF() {

    memset(buf, '\xff', TF_MAX_LEN);
    lenOfCurrentSP = lenOfCurrentUserData = indexOfCurrentUserData = ZERO_IN_SENSE_OF_UNDEF_OR_ERR;
    lenOfCurrentTF = tfHeader.HEADER_SIZE + tfTrailer.HEADER_SIZE;
    indexOfCurrentSP = tfHeader.HEADER_SIZE;
}

uint16_t UplinkEnvelop::beginNewSP() {

    indexOfCurrentUserData = static_cast<uint16_t>(indexOfCurrentSP + spHeader.HEADER_SIZE);
    userDataBuf            = &buf[indexOfCurrentUserData];
    lenOfCurrentSP         = 0; // will be set at commit
    lenOfCurrentUserData   = 0;

    int32_t maxUserLen = TF_MAX_LEN - indexOfCurrentUserData - tfTrailer.HEADER_SIZE;
    if(maxUserLen < 0) maxUserLen = 0;
    return static_cast<uint16_t>(maxUserLen);
}

void UplinkEnvelop::commitSP() {

    if(lenOfCurrentUserData == 0) {   // sp was aborted
        lenOfCurrentSP = 0;
        return;
    }

    lenOfCurrentSP       = static_cast<uint16_t>(spHeader.HEADER_SIZE      + lenOfCurrentUserData);
    spHeader.length      = static_cast<uint16_t>((spHeader.HEADER_SIZE - 6u) + lenOfCurrentUserData - 1u); // Primary-Header-len: 6, Sec-Hea: spHeader.HEADER_SIZE-6
    spHeader.serialize(buf + indexOfCurrentSP);
    indexOfCurrentSP     = static_cast<uint16_t>(indexOfCurrentSP + lenOfCurrentSP);
    lenOfCurrentTF       = static_cast<uint16_t>(lenOfCurrentTF + lenOfCurrentSP);
}


void UplinkEnvelop::commitTF() {

    //addIdleSP(); Not for uplink! for uplink the size of TF is adapted
    tfHeader.frameLength = static_cast<uint16_t>(lenOfCurrentTF -1u);

    tfHeader.serialize(buf);
    tfTrailer.serialize(buf + lenOfCurrentTF - tfTrailer.HEADER_SIZE);
    uint16_t myCrc = crcChecker.computeCRC((uint8_t*)buf, lenOfCurrentTF-2u, CRC_SEED) & 0xffff; // -2? :  2 bytes CRC
    tfTrailer.crc = myCrc;
    tfTrailer.serialize(buf + lenOfCurrentTF - tfTrailer.HEADER_SIZE); // again? now with CRC
}








/*************************** To Read (Receive) Data **********************/

//_______________________________________________________________ check-out for Downlink (code for ground station)


bool DownlinkEnvelop::checkoutTF() {

    indexOfCurrentSP = ZERO_IN_SENSE_OF_UNDEF_OR_ERR;
    tfHeader.deserialize(buf);

    lenOfCurrentTF = DOWNLINK_TF_LEN;

    tfTrailer.deserialize(buf + lenOfCurrentTF - tfTrailer.HEADER_SIZE); // at the end of TF

    uint16_t myCrc = crcChecker.computeCRC((uint8_t*)buf, lenOfCurrentTF-2u, CRC_SEED) & 0xffff; // -2? :  2 bytes CRC
    return (myCrc == tfTrailer.crc);
}


bool DownlinkEnvelop::checkoutNextSP() {

    if(indexOfCurrentSP == ZERO_IN_SENSE_OF_UNDEF_OR_ERR) {
        indexOfCurrentSP = tfHeader.HEADER_SIZE; // CCSDS allows a gap between header and first SP, but I do not!
    } else {
        indexOfCurrentSP = static_cast<uint16_t>(indexOfCurrentSP + lenOfCurrentSP);
    }

    if((indexOfCurrentSP + spHeader.HEADER_SIZE + tfTrailer.HEADER_SIZE) > lenOfCurrentTF)  {
        return false;
    }

    spHeader.deserialize(buf + indexOfCurrentSP);
    lenOfCurrentSP         = static_cast<uint16_t>(spHeader.dataPackLen + 6 + 1); // 6: len of secondady Header
    lenOfCurrentUserData   = static_cast<uint16_t>(lenOfCurrentSP   - spHeader.HEADER_SIZE);
    indexOfCurrentUserData = static_cast<uint16_t>(indexOfCurrentSP + spHeader.HEADER_SIZE);
    userDataBuf            = &buf[indexOfCurrentUserData];

    if(spHeader.typeId != 0 || spHeader.applicationId == 0x7ff ||
            (indexOfCurrentSP + lenOfCurrentSP) > lenOfCurrentTF) {
        return false;
    }
    return true;
}

} // namespace ccsds


