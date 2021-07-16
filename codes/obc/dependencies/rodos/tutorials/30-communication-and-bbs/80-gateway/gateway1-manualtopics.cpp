
#include "rodos.h"
#include "gateway.h"
#include "demo_topics.h"

//Provides a link to an UDP Broadcastnetwork on standart port.
// Topics to send are set manual


UDPInOut udp(-5001);
LinkinterfaceUDP linkinterfaceUDP(&udp);
Gateway gateway1(&linkinterfaceUDP);


class GatewayInitiator1 : public Initiator {
    void init() {
        gateway1.resetTopicsToForward();
        gateway1.addTopicsToForward(&counter1); // you may add up to 3 topics in each line
        gateway1.addTopicsToForward(&counter2);
    }
} gatewayInitiator1;



