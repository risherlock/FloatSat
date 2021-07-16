#include "threadtimings.h"


/*
This is done only to be able to perform timing anylse with
tools/timing-display/
*/

namespace Common {

#define MAX_THREADS 100

uint16_t ThreadTiming::threadTimingListCnt = 0;
ThreadTiming* threadTimingList[MAX_THREADS];

ThreadTiming::ThreadTiming(int64_t st, int64_t per, int64_t len, const char* nm) {
    startTime = st;
    period    = per;
    length    = len;
    name      = nm;

    if(threadTimingListCnt == 0) for (int i = 0; i < MAX_THREADS; i++) threadTimingList[i] = 0;

    if(threadTimingListCnt < MAX_THREADS) threadTimingList[threadTimingListCnt] = this;
    threadTimingListCnt++;
}


                                /********* Timings in nanoseconds ******/
                                /* start        period      duration   */
                                /*smmmuuunnn   smmmuuunnn   smmmuuunnn */
ThreadTiming timingTIMEMANAGER   (1000000000,  1000000000,     4000000, "TIMEMANAGER"  );
ThreadTiming timingXXX           (2188000000,   500000000,     2000000, "XXX"          );
ThreadTiming timingREDUNDANCYMGR (3200000000,  1000000000,     2000000, "REDUNDANCYMGR");
ThreadTiming timingHKE           (3250000000,   500000000,     1000000, "HKCOLLEC"     );
ThreadTiming timingUPLINKREADR   (1300000000,   500000000,    20000000, "UPLINKREADR"  );
ThreadTiming timingDOWNLINKSENDER(5350000000,  1000000000,    22000000, "TFCOMPOSSER"  );
ThreadTiming timingWATCHDOG      ( 400000000,  1000000000,     7000000, "WATCHDOG"     ); // Warning: Firststart < 1 Sec!
ThreadTiming timingTIMEDCMDMGR   (1450000000,  1000000000,    22000000, "TIMEDCMDMGR"  );
ThreadTiming timingBOOTMANAGER   (1600000000,  1000000000,     1000000, "BOOTMANAGER"  );
ThreadTiming timingSURVEILLANCE  (1650000000,   500000000,     1000000, "SURVEILLANCE" );
ThreadTiming timingDRV           (1700000000,   500000000,     1000000, "DRIVECNTRL"   );
ThreadTiming timingMOT           (1750000000,   500000000,     1000000, "DRIVECNTRL"   );

//__________________________________________________________________________________________ only to test
ThreadTiming timingSBANDLINK     ( 800000000,   500000000,     4000000, "SBANDLINK" );
ThreadTiming timingMODEMANAGER   ( 850000000,   400000000,     3000000, "MODEMANAGER" );
ThreadTiming timingPOWERCONTROL  ( 900000000,   500000000,     3000000, "POWERCONTROL" );

}
