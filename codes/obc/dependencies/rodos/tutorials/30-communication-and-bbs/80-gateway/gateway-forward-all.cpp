
#include "rodos.h"
#include "gateway.h"
#include "demo_topics.h"

//Provides a link to a second UDP Broadcastnetwork on a different port.
// Topics to send are set manual

static UDPInOut udp3(-50002);  //A different broadcast network
static LinkinterfaceUDP linkinterfaceUDP3(&udp3);
static Gateway gateway3(&linkinterfaceUDP3, true);
