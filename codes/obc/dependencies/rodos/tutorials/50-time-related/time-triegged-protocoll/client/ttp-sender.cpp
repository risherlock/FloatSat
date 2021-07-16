#include "rodos.h"
#include "../topics.h"
#include "mask.h"

extern int32_t myNodeIdAssignedByServer;

/**
 * Sends a message in the TTP network every 1 second.
 */
class TTPSender : public StaticThread<> {
  public:
    TTPSender() : StaticThread<>("ttpSenderTh") { messageCnt = 0; }

    void run() {

        AT(3*SECONDS); // suspend thread until sync

        TTPMessage nodeMessage;
        nodeMessage.nodeId = myNodeIdAssignedByServer;

        int64_t offset = 4*SECONDS + sysTime.getUTC() - (sysTime.getUTC() % SECONDS) + (myNodeIdAssignedByServer)*250 * MILLISECONDS;
        while(true) {
            int64_t nextTime = TimeModel::computeNextBeat(offset, 1*SECONDS, sysTime.getUTC());

            AT_UTC(nextTime);
            nodeMessage.message = ++messageCnt;
            ttpMessage.publish(nodeMessage);
        }
    }

  private:
    int32_t messageCnt;

} ttpSender;
