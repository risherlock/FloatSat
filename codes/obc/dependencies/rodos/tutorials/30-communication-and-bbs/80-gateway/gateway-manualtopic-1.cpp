
#include "rodos.h"
#include "gateway.h"
#include "demo_topics.h"

//Provides a link to a second UDP Broadcastnetwork on a different port.
// Topics to send are set manual

static UDPInOut udp1(-50001);  //A different broadcast network
static LinkinterfaceUDP linkinterfaceUDP1(&udp1);
static Gateway gateway1(&linkinterfaceUDP1);

class GatewayInitiator1 : public Initiator {
    void init() {
        gateway1.resetTopicsToForward();
        gateway1.addTopicsToForward(&counter1);
    }
} gatewayInitiator1;




