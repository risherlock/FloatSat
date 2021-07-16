
#pragma once

#include "rodos.h"
#include "ccsds/ccsds-envelop.h"


/* Sergio Montenegro
 * Nov 2016
 */


namespace CCSDS {

/*
 * DownlinkFrame: abstraction and simple use of ccsds DownlinkEnvelop
 * ist a TransferFrame, with a single Sourcepackage
 */


/* how to use:
 *   DownlinkFrame myTm;
 *   char* dest  = myTm.beginNewSpTF();
 *   myTm.setVals(321, 1,2,3,4); // (spaceCraftId, apid, serv, subserv, vcid)
 *   serialize_your_data_to(dest);
 *   DownlinkTransferFrame* tf = myTm.commit();
 *   publish_or_send(tf);
 *
 * shortcut:
 *   char* dest  = myTm.beginNewSpTF(1,2,3,4); // (apid, serv, subserv, vcid)
 *   serialize_your_data_to(dest);
 *   ...
 */

struct DownlinkFrame {

    DownlinkEnvelop downlinkEnvelop;

    char* beginNewFrame() { 
        downlinkEnvelop.beginNewTF();
        int len = downlinkEnvelop.beginNewSP();
        downlinkEnvelop.lenOfCurrentUserData = len;
        return downlinkEnvelop.userDataBuf;
    }

   char* beginNewFrame(uint32_t spaceCraftId, int apid, int serv, int subserv, int vcid) {
        beginNewFrame();
        setVals(spaceCraftId, apid, serv, subserv, vcid);
        return downlinkEnvelop.userDataBuf;
    }


    DownlinkTransferFrame* commit() {
        //downlinkEnvelop.lenOfCurrentUserData = MAX_USER_DATA_LEN; // allready set in beginNewFrame()
        downlinkEnvelop.commitSP();
        downlinkEnvelop.commitTF();
        return downlinkEnvelop.getTfPtr();
    }

    void setVcid(int vcid) { 
        downlinkEnvelop.tfHeader.virtualChanId        = vcid;
    }

    void setVals(uint32_t spaceCraftId, int apid, int serv, int subserv, int vcid) {
        downlinkEnvelop.initDefaultTFHeaderAndTrailer(spaceCraftId);
        downlinkEnvelop.initDefaultSPHeader();

        downlinkEnvelop.spHeader.applicationId     = apid;
        downlinkEnvelop.spHeader.service           = serv;
        downlinkEnvelop.spHeader.subservice        = subserv;

        downlinkEnvelop.tfHeader.virtualChanId     = vcid;
    }

};
}
