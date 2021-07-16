#include "rodos.h"
#include "ccsds/ccsds-envelop.h"

#include "stdio.h" // NOT rodos!!! 

/**
 *------------- to recieve  data----------------
 *
 *Ground:  (receives Telemetry)
 *       downlink-transferframe.bf
 *SpaceCraft:  (recieves commands) <<<<<<<<<<<<<<<<<<<<<<<<<<<<
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


/********** For Sapacecraft ************/

using namespace CCSDS;

UplinkEnvelop uplinkEnvelop;
int printfVerbosity = 0; // no prints from rodos

class CCDStests: public Thread {
public:
    void run() {
        printfVerbosity = 100; // print acticated

        FILE* inFile = fopen("uplinktf.bin", "r");
        fread(uplinkEnvelop.getTfPtr(), 1, TF_MAX_LEN, inFile);
        fclose(inFile);

        //_____________________________________________________________________ Transferframe
        bool ok = uplinkEnvelop.checkoutTF();
        PRINTF("\nUplink Transferframe:\n");
        PRINTF("    TF ok = %d, len = %d\n\n", ok, uplinkEnvelop.lenOfCurrentTF);

        PRINTF("    HEADER_SIZE          : %3u\n",  uplinkEnvelop.tfHeader.HEADER_SIZE);
        PRINTF("    version              : %3u\n",  uplinkEnvelop.tfHeader.version);
        PRINTF("    bypassFlag           : %3u\n",  uplinkEnvelop.tfHeader.bypassFlag);
        PRINTF("    controllCommandFlag  : %3u\n",  uplinkEnvelop.tfHeader.controllCommandFlag);
        PRINTF("    spare1               : %3u\n",  uplinkEnvelop.tfHeader.spare1);
        PRINTF("    spacecraftID         : %3u\n",  uplinkEnvelop.tfHeader.spacecraftID);
        PRINTF("    virtualChannelID     : %3u\n",  uplinkEnvelop.tfHeader.virtualChannelID);
        //PRINTF("    spare2               : %3u\n",  uplinkEnvelop.tfHeader.spare2);
        PRINTF("    frameLength          : %3u\n",  uplinkEnvelop.tfHeader.frameLength);
        PRINTF("    frameSequenceNr      : %3u\n",  uplinkEnvelop.tfHeader.frameSequenceNr);
        PRINTF("    sequenceFlags        : %3u\n",  uplinkEnvelop.tfHeader.sequenceFlags);
        PRINTF("    multiplexAceessPoint : %3u\n",  uplinkEnvelop.tfHeader.multiplexAceessPoint);

        //_____________________________________________________________________ Source package
        while((ok = uplinkEnvelop.checkoutNextSP())) {
            //ok = uplinkEnvelop.checkoutNextSP();

            PRINTF("\n\nuplink Sourcepackage:\n");
            PRINTF("Current data extracted from TF & SP: \n");
            PRINTF(" ok = %d, tflen = %d, splen = %d, userdatalen = %d, spIndex = %d\n", ok,
                   uplinkEnvelop.lenOfCurrentTF, uplinkEnvelop.lenOfCurrentSP, uplinkEnvelop.lenOfCurrentUserData,
                   uplinkEnvelop.indexOfCurrentSP);

            PRINTF("UplinkSPHeader :\n");
            PRINTF("    HEADER_SIZE : %3d\n",  uplinkEnvelop.spHeader.HEADER_SIZE);
            PRINTF("    version              : %3u\n",  uplinkEnvelop.spHeader.version);
            PRINTF("    type                 : %3u\n",  uplinkEnvelop.spHeader.type);
            PRINTF("    secondaryHeaderFlag  : %3u\n",  uplinkEnvelop.spHeader.secondaryHeaderFlag);
            PRINTF("    applicationId        : %3u\n",  uplinkEnvelop.spHeader.applicationId);
            PRINTF("    sequenceFlags        : %3u\n",  uplinkEnvelop.spHeader.sequenceFlags);
            PRINTF("    sequenceCounter      : %3u\n",  uplinkEnvelop.spHeader.sequenceCounter);
            PRINTF("    length               : %3u\n",  uplinkEnvelop.spHeader.length);
            PRINTF("    pusSecondaryHderFlag : %3u\n",  uplinkEnvelop.spHeader.pusSecondaryHeaderFlag);
            PRINTF("    pusVersion           : %3u\n",  uplinkEnvelop.spHeader.pusVersion);
            PRINTF("    ackType              : %3u\n",  uplinkEnvelop.spHeader.ackType);
            PRINTF("    serviceType          : %3u\n",  uplinkEnvelop.spHeader.serviceType);
            PRINTF("    serviceSubtype       : %3u\n",  uplinkEnvelop.spHeader.serviceSubtype);
            PRINTF("    sourceID             : %3u\n",  uplinkEnvelop.spHeader.sourceID);

            PRINTF("      %d bytes of user data: ", uplinkEnvelop.lenOfCurrentUserData);
            for(int i = 0; i < uplinkEnvelop.lenOfCurrentUserData; i++) PRINTF("%c", uplinkEnvelop.userDataBuf[i]);

        }

        PRINTF("\n\nterminating with abort\n");
        hwResetAndReboot();
    }
} ccsdsTests;


