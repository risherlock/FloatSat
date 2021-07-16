
#include "rodos.h"
#include "ccsds/ccsds-envelop.h"
#include "ccsds/downlink-frame.h"
#include "stdio.h" // NOT rodos!!! 


/********** For spacecraft ************/

int printfVerbosity = 0; // no prints from rodos

using namespace CCSDS;

DownlinkFrame myTm; // Exaclty one SP per TF, make all simpler!


class CCDStests: public Thread {
public:
    void run() {
        printfVerbosity = 100; // print acticated
        PRINTF("it writes a singel downlink sp & Tranferframe to file downlinktf.bin\n");
       
        char* dest = myTm.beginNewFrame(1,2,3,4); // (apid, serv, subserv, vcid)
        strcpy(dest, "This is a single sp in a single tf, rest ist empty \0\0\0");
        DownlinkTransferFrame* tf = myTm.commit();

        //______________________________________________________________________ write to file

        FILE* outFile = fopen("downlinktf.bin", "w+");
        fwrite(tf, 1, TF_MAX_LEN, outFile);
        fclose(outFile);

        PRINTF("\n\nterminating in 1 second\n");
        AT(NOW() + 1*SECONDS);
        hwResetAndReboot();
    }

} ccsdsTests;


