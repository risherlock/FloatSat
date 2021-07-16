
#include "rodos.h"
#include "gateway.h"

//Provides a link to an UDP Broadcastnetwork on standart port.
//Uses a TopicReporter to tell other Gateways whitch topics to send

static UDPInOut udp(-5001);
static LinkinterfaceUDP linkinterfaceUDP(&udp);
static Gateway gateway1(&linkinterfaceUDP);

static TopicReporter topicReporter;

class GatewayInitiator : public Initiator {
    void init() {
        topicReporter.addGateway(&gateway1);
    }
} gatewayInitiator;




