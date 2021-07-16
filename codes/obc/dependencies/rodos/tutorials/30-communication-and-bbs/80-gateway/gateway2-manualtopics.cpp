
#include "rodos.h"
#include "gateway.h"
#include "demo_topics.h"

//Provides a link to a second UDP Broadcastnetwork on a different port.
// Topics to send are set manual

UDPInOut udp2(-50002);  //A different broadcast network
LinkinterfaceUDP linkinterfaceUDP2(&udp2);
Gateway gateway2(&linkinterfaceUDP2);


class GatewayInitiator2 : public Initiator {
    void init() {
        gateway2.resetTopicsToForward();
        gateway2.addTopicsToForward(&counter1,&counter2);
    }
} gatewayInitiator2;




