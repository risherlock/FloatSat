

#include "rodos.h"
#include "ccsds-envelop.h"

// envelop for Spacesegment
//     Allmost (but only allmost) identic to the ground segment code

namespace CCSDS {

CRC             crcChecker;
Application     applicationSpacecraft("CCSDS-spacesegmentsegment-envelop");

/*************************** To Write (Send) Data **********************/


uint32_t DownlinkApidSequenceCounter[2048] = {0,0,0,0,0,0,0,0}; // for 11 bit, we init only 8 but rest will be 0
uint32_t DownlinkVirtualChanFrameCnt[8]    = {0, 0, 0, 0, 0, 0, 0, 0};
uint32_t DownlinkMasterFramecounter        = 0;

//______________________________________________________ init Downlinks with default values (Code for spacecraft)

void DownlinkEnvelop::initDefaultTFHeaderAndTrailer(uint16_t spaceCraftId) {

    memset(&tfHeader,  '\0', sizeof(tfHeader));
    memset(&tfTrailer, '\0', sizeof(tfTrailer));

    tfHeader.spaceCraftId        = spaceCraftId;
    tfHeader.virtualChanId       = 7;   //  0: Real time, 1 history, 7 idle
    tfHeader.opControlFlag       = 1;   //  1 -> at the end of the TF is a 32-bit CLCW command status word
    tfHeader.segmentLenId        = 0x3;

    tfTrailer.commandOpProcedure = 1;  // OP in effect recommend 01
    lenOfCurrentTF               = DOWNLINK_TF_LEN; // constant for each mission

}

void DownlinkEnvelop::initDefaultSPHeader() {

    // primary header  allways 6 bytes
    memset(&spHeader, '\0', sizeof(spHeader));
    spHeader.secHeaderFlag       = 1;     //  1 -> secondary header follows
    spHeader.groupingFlags       = 0x3;   //  00: cont packet, 01: first one, 10: last one, 11: no groupping
    spHeader.dataPackLen         = 9;     //  len(secondaryheader + userdata) - 1 , in our case here 10 + userdatalen - 1

    // Secondary header: Warning: not exactly defined in ccsds, in this case 10 bytes
    spHeader.service             = 3;     // 3  = normal bus telemetry
    spHeader.subservice          = 25;    // 25 = Automatic HKD
}

//_______________________________________________________________ commits for downlink (code for spacecraft)

void DownlinkEnvelop::beginNewTF() {

    memset(buf, '\xff', TF_MAX_LEN);
    lenOfCurrentSP = lenOfCurrentUserData = indexOfCurrentUserData = ZERO_IN_SENSE_OF_UNDEF_OR_ERR;
    lenOfCurrentTF = DOWNLINK_TF_LEN;
    indexOfCurrentSP = tfHeader.HEADER_SIZE;
}

uint16_t  DownlinkEnvelop::beginNewSP() {

    indexOfCurrentUserData = static_cast<uint16_t>(indexOfCurrentSP + spHeader.HEADER_SIZE);
    userDataBuf            = &buf[indexOfCurrentUserData];
    lenOfCurrentSP         = 0; // will be set bei commit
    lenOfCurrentUserData   = 0;

    int32_t maxUserLen = DOWNLINK_TF_LEN - indexOfCurrentUserData - tfTrailer.HEADER_SIZE;
    if(maxUserLen < 0) maxUserLen = 0;
    return static_cast<uint16_t>(maxUserLen);
}

void DownlinkEnvelop::commitSP() {

    if(lenOfCurrentUserData == 0) {   // sp was aborted
        lenOfCurrentSP = 0;
        return;
    }

    lenOfCurrentSP         = static_cast<uint16_t>(spHeader.HEADER_SIZE + lenOfCurrentUserData);
    spHeader.dataPackLen   = static_cast<uint16_t>(lenOfCurrentSP - 6u -1u); // 6: len of primary downlink sp header

    int64_t timeNow            = sysTime.getUTC();
    spHeader.timeStampSeconds  = (uint32_t)(timeNow/SECONDS);
    spHeader.timeStampFraction = (uint16_t)( ((double)(timeNow%SECONDS) / (double)SECONDS) * 64*1024);
    spHeader.sourceSeqCnt      = static_cast<uint16_t>(DownlinkApidSequenceCounter[spHeader.applicationId & 2047]++);

    spHeader.serialize(buf + indexOfCurrentSP);
    indexOfCurrentSP = static_cast<uint16_t>(indexOfCurrentSP + lenOfCurrentSP);
    // lenOfCurrentTF += lenOfCurrentSP; Not for downlink! for downlink it is a constat
}


void DownlinkEnvelop::commitTF() {

    addIdleSP();

    tfHeader.masterChanFrameCnt  = static_cast<uint8_t>(DownlinkMasterFramecounter++);
    tfHeader.virtualChanFrameCnt = static_cast<uint8_t>(DownlinkVirtualChanFrameCnt[tfHeader.virtualChanId & 7]++);
    tfHeader.serialize(buf);

    commitClcwToTF();
}


void DownlinkEnvelop::commitClcwToTF() {
    tfTrailer.serialize(buf + lenOfCurrentTF - tfTrailer.HEADER_SIZE);
    uint16_t myCrc = crcChecker.computeCRC((uint8_t*)buf, lenOfCurrentTF-2u, CRC_SEED) & 0xffff; // -2? :  2 bytes CRC
    tfTrailer.crc = myCrc;
    tfTrailer.serialize(buf + lenOfCurrentTF - tfTrailer.HEADER_SIZE); // again? now with CRC
}

void DownlinkEnvelop::addIdleSP() {      /** The rest to the end of the TF has to be an iddle SP **/
    uint16_t restLen  =  beginNewSP();
    if(restLen  == 0) return; // there is no room for a SP. rest stays in 0xff....
    initDefaultSPHeader(); // idle
    spHeader.applicationId = 0x7ff;
    lenOfCurrentUserData = restLen;
    commitSP();
}






/*************************** To Read (Receive) Data **********************/

//_______________________________________________________________ check-out for Uplink (code for spacecraft)


bool UplinkEnvelop::checkoutTF() {

    indexOfCurrentSP = ZERO_IN_SENSE_OF_UNDEF_OR_ERR;
    tfHeader.deserialize(buf);

    lenOfCurrentTF = static_cast<uint16_t>(tfHeader.frameLength + 1);

    tfTrailer.deserialize(buf + lenOfCurrentTF - tfTrailer.HEADER_SIZE); // at the end of TF

    if(lenOfCurrentTF > TF_MAX_LEN)           return false;
    if(lenOfCurrentTF < tfHeader.HEADER_SIZE) return false;

    uint16_t myCrc = crcChecker.computeCRC((uint8_t*)buf, lenOfCurrentTF-2u, CRC_SEED) & 0xffff; // -2? :  2 bytes CRC
    return (myCrc == tfTrailer.crc);
}


bool UplinkEnvelop::checkoutNextSP() {

    if(indexOfCurrentSP == ZERO_IN_SENSE_OF_UNDEF_OR_ERR) {
        indexOfCurrentSP = tfHeader.HEADER_SIZE; // CCSDS allows a gap between header and first SP, but I do not!
    } else {
        indexOfCurrentSP = static_cast<uint16_t>(indexOfCurrentSP + lenOfCurrentSP);
    }

    if((indexOfCurrentSP + spHeader.HEADER_SIZE + tfTrailer.HEADER_SIZE) > lenOfCurrentTF)  {
        return false;
    }

    spHeader.deserialize(buf + indexOfCurrentSP);
    lenOfCurrentUserData   = static_cast<uint16_t>(spHeader.length  + 1 - (spHeader.HEADER_SIZE - 6)); // HEADER_SIZE - 6 = sec-header-len
    lenOfCurrentSP         = static_cast<uint16_t>(lenOfCurrentUserData + spHeader.HEADER_SIZE);
    indexOfCurrentUserData = static_cast<uint16_t>(indexOfCurrentSP + spHeader.HEADER_SIZE);
    userDataBuf            = &buf[indexOfCurrentUserData];

    if(spHeader.applicationId == 0x7ff ||
            (indexOfCurrentSP + lenOfCurrentSP) > lenOfCurrentTF) {
        return false;
    }
    return true;
}

} // namespace ccsds


