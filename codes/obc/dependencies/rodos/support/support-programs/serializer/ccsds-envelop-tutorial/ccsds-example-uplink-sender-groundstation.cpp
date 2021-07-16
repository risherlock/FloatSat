
#include "rodos.h"
#include "ccsds/ccsds-envelop.h"

#include "stdio.h" // NOT rodos!!! 

/**
 *---------------- to Send data ---------------
 *
 *Ground: (Sends commands)
 *        uplink-transferframe.bf   <<<<<<<<<<<<<<<<<<<<<<
 *SpaceCraft:(send Telemetry)
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

/********** For ground ************/

using namespace CCSDS;

#define PRINT_ENVELOP_INFOSxx(title)                                                                 \
    PRINTF(" %s  tflen = %4d, splen = %2d, userdatalen = %2d, spIndex = %2d, userDataIndex = %2d\n", \
        title, uplinkEnvelop.lenOfCurrentTF, uplinkEnvelop.lenOfCurrentSP,                           \
        uplinkEnvelop.lenOfCurrentUserData, uplinkEnvelop.indexOfCurrentSP,                          \
        uplinkEnvelop.indexOfCurrentUserData);

#define PRINT_ENVELOP_INFOS(title)                                               \
    PRINTF(" %s  tflen = %4d, splen = %2d, userdatalen = %2d, spIndex = %2d\n",  \
        title, uplinkEnvelop.lenOfCurrentTF, uplinkEnvelop.lenOfCurrentSP,       \
        uplinkEnvelop.lenOfCurrentUserData, uplinkEnvelop.indexOfCurrentSP);


UplinkEnvelop uplinkEnvelop;

int printfVerbosity = 0; // no prints from rodos



void writeSP(const char* stringToSend) {

    int  maxlen = uplinkEnvelop.beginNewSP();
    PRINTF("Max sp len = %d\n", maxlen);
    PRINT_ENVELOP_INFOS("after beginNewSP :");

    strcpy(uplinkEnvelop.userDataBuf, stringToSend);
    uplinkEnvelop.lenOfCurrentUserData = strlen(stringToSend);

    uplinkEnvelop.commitSP();
    PRINT_ENVELOP_INFOS("after CommitSP   :");
}

class CCDStests: public Thread {
public:
    void run() {
        printfVerbosity = 100; // print acticated
        PRINTF("it writes a singel downlink Tranferframe to file uplinktf.bin\n");

        uplinkEnvelop.beginNewTF();
        uplinkEnvelop.initDefaultTFHeaderAndTrailer();
        uplinkEnvelop.initDefaultSPHeader();

        //_____________________________________________________________________ Init My defaults, can be modified later

        //            _______
        uplinkEnvelop.spHeader.version                 = 1;
        //uplinkEnvelop.spHeader.type                    = 2;  use from default
        // uplinkEnvelop.spHeader.secondaryHeaderFlag     = 0;  use from default
        uplinkEnvelop.spHeader.applicationId           = 3;
        // uplinkEnvelop.spHeader.sequenceFlags           = 4;  use from default
        uplinkEnvelop.spHeader.sequenceCounter         = 5;
        // uplinkEnvelop.spHeader.length                  = 6;  let commit set it
        uplinkEnvelop.spHeader.pusSecondaryHeaderFlag  = 7;
        // uplinkEnvelop.spHeader.pusVersion              = 8; use from default
        // uplinkEnvelop.spHeader.ackType                 = 9; use from default
        uplinkEnvelop.spHeader.serviceType             = 10;
        uplinkEnvelop.spHeader.serviceSubtype          = 11;
        uplinkEnvelop.spHeader.sourceID                = 12;

        //            ________
        uplinkEnvelop.tfHeader.version               = 1;
        //uplinkEnvelop.tfHeader.bypassFlag            = 2;  use from default
        //uplinkEnvelop.tfHeader.controllCommandFlag   = 3;  use from default
        uplinkEnvelop.tfHeader.spare1                = 4;
        //uplinkEnvelop.tfHeader.spacecraftID          = 5;  use from default
        uplinkEnvelop.tfHeader.virtualChannelID      = 6;
        //uplinkEnvelop.tfHeader.spare2                = 7;
        //uplinkEnvelop.tfHeader.frameLength           = 8;  let commit set it
        uplinkEnvelop.tfHeader.frameSequenceNr       = 9;
        //uplinkEnvelop.tfHeader.sequenceFlags         =10;  use from default
        //uplinkEnvelop.tfHeader.multiplexAceessPoint  =11;  use from default

        //_____________________________________________________________________ Source packages

        PRINTF("\n_____________________________SP 1: \n");
        writeSP("Hello World");

        PRINTF("\n_____________________________SP 2: \n");
        writeSP("abcdefghijklmnopqrstuvwxyz");

        PRINTF("\n_____________________________SP 3: \n");
        writeSP("zyxwutsrqponmlkjihgfedcba");

        //_____________________________________________________________________ Transferframe

        uplinkEnvelop.commitTF();
        PRINTF("\n_____________________________TF: \n");
        PRINT_ENVELOP_INFOS("after CommitTF   :");

        //______________________________________________________________________ write to file

        FILE* outFile = fopen("uplinktf.bin", "w+");
        fwrite(uplinkEnvelop.getTfPtr(), 1, uplinkEnvelop.lenOfCurrentTF, outFile);
        fclose(outFile);

        PRINTF("\n\nterminating in 1 second\n");
        AT(NOW() + 1*SECONDS);
        hwResetAndReboot();
    }

} ccsdsTests;


