
#include "rodos.h"
#include "gateway.h"
#include "demo_topics.h"

//Provides a link to a second UDP Broadcastnetwork on a different port.
// Topics to send are set manual

static UDPInOut udp2(-50002);  //A different broadcast network
static LinkinterfaceUDP linkinterfaceUDP2(&udp2);
static Gateway gateway2(&linkinterfaceUDP2);


class GatewayInitiator2 : public Initiator {
    void init() {
        gateway2.resetTopicsToForward();
        gateway2.addTopicsToForward(&counter2);
    }
} gatewayInitiator2;




