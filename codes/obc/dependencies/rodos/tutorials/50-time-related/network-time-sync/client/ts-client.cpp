#include "rodos.h"
#include "../topics.h"
#include "mask.h"

/***********************************************/

static Application receiverName("timeSyncClientApp", 1100);

/**
 * Implements a simple SNTP client.
 *  The client sends a server time sync request every 15 seconds. The client
 * receives the server response immediatelly.
 * The client responds to a killall interrupt and prints its <pre>NOW()</pre> time
 * + offset. It should be almost equal to the server <pre>NOW()</pre> time.
 *
 * @author Chavdar Iliev (uni Würzburg)
 */

class TimeSyncClient : public Subscriber, public StaticThread<>, public Putter {
  public:
    double timeAtInterrupt = 0;
    TimeSyncClient() : Subscriber(serverResponse, "timeSyncClient"), StaticThread<>("timeSyncClientTh"),
                       nodeNumber(0), requestCnt(0), ownTS{ 0, 0 } {}

    /** here we send the request to the server */
    void run() {
        nodeNumber = getNodeNumber();
        TIME_LOOP(2 * SECONDS, 15 * SECONDS) {
            ownTS.sendTS = NOW(); // Warning: We send local time and get server-UTC, then we compute our UTC
            clientRequest.publish(++requestCnt);
        }
    }

    /** here we process the server response  See "explanation.pdf" **/
    uint32_t put([[gnu::unused]] const uint32_t _topicId, [[gnu::unused]] const size_t _len, void* _msg, [[gnu::unused]] const NetMsgInfo& _netMsgInfo) {
        TimeSyncResponse msg = *(TimeSyncResponse*)_msg;
        if(msg.clientNodeNr != nodeNumber || msg.clientRequestCnt != requestCnt) return 0;

        ownTS.rcvTS                = NOW();
        int64_t roundTrip          = ownTS.rcvTS - ownTS.sendTS;
        int64_t reaction           = msg.ntspTimes.sendTS - msg.ntspTimes.rcvTS;
        int64_t oneWayTransmission = (roundTrip - reaction) / 2;
        int64_t offset             = (msg.ntspTimes.rcvTS - ownTS.sendTS) - oneWayTransmission;

        sysTime.setUTCDeltaTime(offset);

        PRINTF(REQ "%d" S_SENT_TS "%9.6f" S_RCV_TS "%9.6f" C_SENT_TS "%9.6f" C_RCV_TS "%9.6f" OFFSET "%9.6f" DELAY "%9.6f",
               static_cast<int>(requestCnt), toMillis(msg.ntspTimes.sendTS), toMillis(msg.ntspTimes.rcvTS),
               toMillis(ownTS.sendTS), toMillis(ownTS.rcvTS), toMillis(offset), toMillis(oneWayTransmission));
        

        return 1;
    }

    /** Just for debug: Print the utc time as response to the (global) interrupt **/
    bool putGeneric([[gnu::unused]] const uint32_t _topicId, [[gnu::unused]] const size_t _msgLen, [[gnu::unused]] const void* _msg, [[gnu::unused]] const NetMsgInfo& _netMsgInfo) {
        timeAtInterrupt = utcMillisecsNow();
        resume();
        return true;
    }

  private:
    int32_t   nodeNumber;
    int32_t   requestCnt;
    NtspTimes ownTS; ///< Client send / receive time stamps (T0)
} timeSyncClient;

/// Interupt
static Subscriber nameNotImportant01(interruptSigterm, timeSyncClient, "resumeFromInterruptClient");


///Displays time
class TimeDisplay : public StaticThread<> {
    void run() {
        TIME_LOOP(5*SECONDS, 100*MILLISECONDS) {
            PRINTF(UTC_TIME "%3.6f", (double)sysTime.getUTC()/SECONDS);
        }
    }
} timeDisplay;
