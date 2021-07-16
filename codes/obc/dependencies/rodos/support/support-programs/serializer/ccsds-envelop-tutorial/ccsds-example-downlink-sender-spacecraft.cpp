
#include "rodos.h"
#include "ccsds/ccsds-envelop.h"

#include "stdio.h" // NOT rodos!!! 

/**
 *---------------- to Send data ---------------
 *
 *Ground: (Sends commands)
 *        uplink-transferframe.bf
 *SpaceCraft:(send Telemetry)       <<<<<<<<<<<<<<<
 *        downlink-transferframe.bf
 *
 * initDefaultTFHeaderAndTrailer();
 * initDefaultSPHeader();
 *
 *loop {
 *        beginNewTF
 *        loop {
 *                maxlen = beginNewSP();
 *                if(maxlen < whatINeed) break;
 *                wirte data to userDataBuf[0 ..  lenOfCurrentUserData-1]
 *                lenOfCurrentUserData = myLen;
 *                set  required sp-header parameter
 *                commitSP();
 *        }
 *        set required ft-header parameter
 *        commitTF();
 *        send (buf, tflen);
 *}
 **/

/********** For spacecraft ************/

using namespace CCSDS;

#define PRINT_ENVELOP_INFOSxx(title)                                                                 \
    PRINTF(" %s  tflen = %4d, splen = %2d, userdatalen = %2d, spIndex = %2d, userDataIndex = %2d\n", \
        title, downlinkEnvelop.lenOfCurrentTF, downlinkEnvelop.lenOfCurrentSP,                       \
        downlinkEnvelop.lenOfCurrentUserData, downlinkEnvelop.indexOfCurrentSP,                      \
        downlinkEnvelop.indexOfCurrentUserData);

#define PRINT_ENVELOP_INFOS(title)                                               \
    PRINTF(" %s  tflen = %4d, splen = %2d, userdatalen = %2d, spIndex = %2d\n",  \
        title, downlinkEnvelop.lenOfCurrentTF, downlinkEnvelop.lenOfCurrentSP,   \
        downlinkEnvelop.lenOfCurrentUserData, downlinkEnvelop.indexOfCurrentSP);


DownlinkEnvelop downlinkEnvelop;
int printfVerbosity = 0; // no prints from rodos



void writeSP(const char* stringToSend) {

    int  maxlen = downlinkEnvelop.beginNewSP();
    PRINTF("Max sp len = %d\n", maxlen);
    PRINT_ENVELOP_INFOS("after beginNewSP :");

    strcpy(downlinkEnvelop.userDataBuf, stringToSend);
    downlinkEnvelop.lenOfCurrentUserData = strlen(stringToSend);

    downlinkEnvelop.commitSP();
    PRINT_ENVELOP_INFOS("after CommitSP   :");
}

class CCDStests: public Thread {
public:
    void run() {
        printfVerbosity = 100; // print acticated
        PRINTF("it writes a singel downlink Tranferframe to file downlinktf.bin\n");

        downlinkEnvelop.beginNewTF();
        downlinkEnvelop.initDefaultTFHeaderAndTrailer();
        downlinkEnvelop.initDefaultSPHeader();

        //_____________________________________________________________________ Init My defaults, can be modified later

        //              _______
        downlinkEnvelop.spHeader.version           = 1;
        downlinkEnvelop.spHeader.typeId            = 2;
        // downlinkEnvelop.spHeader.secHeaderFlag     = 1;  use from default
        downlinkEnvelop.spHeader.applicationId     = 3;
        // downlinkEnvelop.spHeader.groupingFlags     = 0;  use from default
        // downlinkEnvelop.spHeader.sourceSeqCnt      = 4;  will be set bei commit
        // downlinkEnvelop.spHeader.dataPackLen       = 4;  let commit set it
        downlinkEnvelop.spHeader.secondaryFlag     = 0;
        downlinkEnvelop.spHeader.pusVersion        = 5;
        downlinkEnvelop.spHeader.spare             = 6;
        downlinkEnvelop.spHeader.service           = 7;
        downlinkEnvelop.spHeader.subservice        = 8;
        downlinkEnvelop.spHeader.destination       = 9;
        //downlinkEnvelop.spHeader.timeStampSeconds  =10; set in commit
        //downlinkEnvelop.spHeader.timeStampFraction =11; set in commit

        //              _______
        downlinkEnvelop.tfHeader.version              =  1;
        // downlinkEnvelop.tfHeader.spaceCraftId         =  2; use from default
        downlinkEnvelop.tfHeader.virtualChanId        =  3;
        // downlinkEnvelop.tfHeader.opControlFlag        =  0;  use from default
        //downlinkEnvelop.tfHeader.masterChanFrameCnt   =  4; will be set bei commit
        //downlinkEnvelop.tfHeader.virtualChanFrameCnt  =  5; will be set bei commit
        // downlinkEnvelop.tfHeader.secondHeaderFlag     =  1; use from default
        // downlinkEnvelop.tfHeader.synchFlag            =  0; use from default
        // downlinkEnvelop.tfHeader.packetOrderFlag      =  0; use from default
        // downlinkEnvelop.tfHeader.segmentLenId         =  7;  use from default
        // downlinkEnvelop.tfHeader.firstHeaderPtr       =  0;  in our mission has to be 0! use default

        //_____________________________________________________________________ Source packages

        PRINTF("\n_____________________________SP 1: \n");
        writeSP("Hello World");

        PRINTF("\n_____________________________SP 2: \n");
        writeSP("abcdefghijklmnopqrstuvwxyz");

        PRINTF("\n_____________________________SP 3: \n");
        writeSP("zyxwutsrqponmlkjihgfedcba");

        //_____________________________________________________________________ Transferframe

        downlinkEnvelop.commitTF();
        PRINTF("\n_____________________________TF: \n");
        PRINT_ENVELOP_INFOS("after CommitTF   :");

        //______________________________________________________________________ write to file

        FILE* outFile = fopen("downlinktf.bin", "wb+");
        fwrite(downlinkEnvelop.getTfPtr(), 1, downlinkEnvelop.lenOfCurrentTF, outFile);
        fclose(outFile);

        PRINTF("\n\nterminating in 1 second\n");
        AT(NOW() + 1*SECONDS);
        hwResetAndReboot();
    }

} ccsdsTests;


