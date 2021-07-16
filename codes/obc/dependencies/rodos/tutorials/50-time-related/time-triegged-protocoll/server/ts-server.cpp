#include "rodos.h"
#include "../topics.h"
#include "mask.h"

/***********************************************/

static Application      senderName("timeSyncServerApp", 1000);
static TimeSyncResponse response;
static int32_t          requestCount = 0;

#define MAXNODES 10
int32_t registeredNodes[MAXNODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int32_t getNodeId(int32_t searchId) {
    for(int i = 0; i < MAXNODES; i++) {
        if(registeredNodes[i] == 0) registeredNodes[i] = searchId;
        if(registeredNodes[i] == searchId) return i;
    }
    return MAXNODES;
}


/**
 * Implements a simple SNTP server. The server can process requests from multiple clients.
 *  <p>The clients send their current request count. The server fills the <pre>clientNodeNr</pre>
 * field in the <pre>TimeSyncResponse</pre> and posts the response immediatly.
 *
 * @author Chavdar Iliev (uni Würzburg)
 */
class TimeSyncServer : public Subscriber, public Putter {
  public:
    TimeSyncServer() : Subscriber(clientRequest, "timeSyncServer") {}

    uint32_t put([[gnu::unused]] const uint32_t _topicId, [[gnu::unused]] const size_t _len, void* _msg, [[gnu::unused]] const NetMsgInfo& _netMsgInfo) {
        int32_t requestId               = *(int32_t*)_msg;
        response.ntspTimes.rcvTS        = sysTime.getUTC(); // by real time server, set by hardware at arrival
        response.clientRequestCnt       = requestId;
        response.clientNodeNr           = _netMsgInfo.senderNode;
        response.nodeIdAssignedByServer = getNodeId(_netMsgInfo.senderNode);

        PRINTF(CL_NODE "%d" REQ "%d", static_cast<int>(response.clientNodeNr), static_cast<int>(++requestCount));

        response.ntspTimes.sendTS = sysTime.getUTC(); // by real time server, set by hardware at deliver time
        serverResponse.publish(response);
        return 1;
    }

    /** Just for debug: Print the utc time as response to the (global) interrupt **/
    bool putGeneric([[gnu::unused]] const uint32_t _topicId, [[gnu::unused]] const size_t _msgLen, [[gnu::unused]] const void* _msg, [[gnu::unused]] const NetMsgInfo& _netMsgInfo) {
        PRINTF(INT_TIME_SRV "%9.6f", utcMillisecsNow());
        return true;
    }
} timeSyncServer;

/// Intterupt
static Subscriber nameNotImportant(interruptSigterm, timeSyncServer, "resumeFromInterruptServer");
