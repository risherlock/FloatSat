
#include "rodos.h"
#include "gateway.h"

static UDPInOut udp(-50000);
static LinkinterfaceUDP linkinterfaceUDP(&udp);
static Gateway gateway1(&linkinterfaceUDP, true);


namespace RODOS {
extern int32_t myNodeNr;
}
static class SetNodeNr : public Initiator {
    void init() {
        // RODOS::myNodeNr = 7; WARNING! This blocks gateway receivers,
        // because this file is used for sender and receivers;
        // if the nodenr of the sender is identical to the receiver, the message is discarded.
        //xprintf(" initiator nodenr = %ld\n", RODOS::myNodeNr);
    }
} setNodeNr;
