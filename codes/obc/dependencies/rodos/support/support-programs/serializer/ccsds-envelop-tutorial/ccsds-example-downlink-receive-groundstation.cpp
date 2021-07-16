#include "rodos.h"
#include "ccsds/ccsds-envelop.h"

#include "stdio.h" // NOT rodos!!! 

/**
 *------------- to recieve  data----------------
 *
 *Ground:  (receives Telemetry)    <<<<<<<<<<<<<<<<<<<<<<<<<<<<
 *       downlink-transferframe.bf
 *SpaceCraft:  (recieves commands)
 *        uplink-transferframe.bf
 *
 *loop {
 *        checkoutTF();
 *        use some tf-header parameter
 *        loop {
 *                ok = checkoutSP();
 *                if(!ok) break;
 *                use some sp-header parameter
 *                read data from userDataBuf[0 ..  lenOfCurrentUserData-1]
 *        }
 *}
 **/


/********** For groundsegment ************/

using namespace CCSDS;

DownlinkEnvelop downlinkEnvelop;
int printfVerbosity = 0; // no prints from rodos

class CCDStests: public Thread {
public:
    void run() {
        printfVerbosity = 100; // print acticated

        FILE* inFile = fopen("downlinktf.bin", "r");
        fread(downlinkEnvelop.getTfPtr(), 1, TF_MAX_LEN, inFile);
        fclose(inFile);

        //_____________________________________________________________________ Transferframe
        bool ok = downlinkEnvelop.checkoutTF();
        PRINTF("\nDownlink Transferframe:\n");
        PRINTF("    TF ok = %d, len = %d\n\n", ok, downlinkEnvelop.lenOfCurrentTF);

        PRINTF("    HEADER_SIZE          : %3u\n\n",downlinkEnvelop.tfHeader.HEADER_SIZE);
        PRINTF("    version              : %3u\n",  downlinkEnvelop.tfHeader.version);
        PRINTF("    spaceCraftId         : %3u\n",  downlinkEnvelop.tfHeader.spaceCraftId);
        PRINTF("    virtualChanId        : %3u\n",  downlinkEnvelop.tfHeader.virtualChanId);
        PRINTF("    opControlFlag        : %3u\n",  downlinkEnvelop.tfHeader.opControlFlag);
        PRINTF("    masterChanFrameCnt   : %3u\n",  downlinkEnvelop.tfHeader.masterChanFrameCnt);
        PRINTF("    virtualChanFrameCnt  : %3u\n",  downlinkEnvelop.tfHeader.virtualChanFrameCnt);
        PRINTF("    secondHeaderFlag     : %3u\n",  downlinkEnvelop.tfHeader.secondHeaderFlag);
        PRINTF("    synchFlag            : %3u\n",  downlinkEnvelop.tfHeader.synchFlag);
        PRINTF("    packetOrderFlag      : %3u\n",  downlinkEnvelop.tfHeader.packetOrderFlag);
        PRINTF("    segmentLenId         : %3u\n",  downlinkEnvelop.tfHeader.segmentLenId);
        PRINTF("    firstHeaderPtr       : %3u\n",  downlinkEnvelop.tfHeader.firstHeaderPtr);

        //_____________________________________________________________________ Source package
        while((ok = downlinkEnvelop.checkoutNextSP())) {
            //ok = downlinkEnvelop.checkoutNextSP();

            PRINTF("\n\nDownlinkSPHeader :\n");
            PRINTF("Current data extracted from TF & SP: \n");
            PRINTF("ok = %d, tflen = %d, splen = %d, userdatalen = %d, spIndex = %d\n", ok,
                   downlinkEnvelop.lenOfCurrentTF, downlinkEnvelop.lenOfCurrentSP, downlinkEnvelop.lenOfCurrentUserData,
                   downlinkEnvelop.indexOfCurrentSP);

            PRINTF("    HEADER_SIZE          : %3u\n\n",downlinkEnvelop.spHeader.HEADER_SIZE);
            PRINTF("    version              : %3u\n",  downlinkEnvelop.spHeader.version);
            PRINTF("    typeId               : %3u\n",  downlinkEnvelop.spHeader.typeId);
            PRINTF("    secHeaderFlag        : %3u\n",  downlinkEnvelop.spHeader.secHeaderFlag);
            PRINTF("    applicationId        : %3u\n",  downlinkEnvelop.spHeader.applicationId);
            PRINTF("    groupingFlags        : %3u\n",  downlinkEnvelop.spHeader.groupingFlags);
            PRINTF("    sourceSeqCnt         : %3u\n",  downlinkEnvelop.spHeader.sourceSeqCnt);
            PRINTF("    dataPackLen          : %3u\n",  downlinkEnvelop.spHeader.dataPackLen);
            PRINTF("    secondaryFlag        : %3u\n",  downlinkEnvelop.spHeader.secondaryFlag);
            PRINTF("    pusVersion           : %3u\n",  downlinkEnvelop.spHeader.pusVersion);
            PRINTF("    spare                : %3u\n",  downlinkEnvelop.spHeader.spare);
            PRINTF("    service              : %3u\n",  downlinkEnvelop.spHeader.service);
            PRINTF("    subservice           : %3u\n",  downlinkEnvelop.spHeader.subservice);
            PRINTF("    destination          : %3u\n",  downlinkEnvelop.spHeader.destination);
            PRINTF("    timeStampSeconds     : %3u\n",  downlinkEnvelop.spHeader.timeStampSeconds);
            PRINTF("    timeStampFraction    : %3u\n",  downlinkEnvelop.spHeader.timeStampFraction);

            PRINTF("      %d bytes of user data: ", downlinkEnvelop.lenOfCurrentUserData);
            for(int i = 0; i < downlinkEnvelop.lenOfCurrentUserData; i++) PRINTF("%c", downlinkEnvelop.userDataBuf[i]);

        }

        PRINTF("\n\nterminating with abort\n");
        hwResetAndReboot();
    }
} ccsdsTests;


