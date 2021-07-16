
#include "rodos.h"
#include "gateway.h"

Topic<long>   ping(-1, "ping1");
Topic<long>   pong(-1, "pong2");

static UDPInOut udp(-50000);
static LinkinterfaceUDP linkinterfaceUDP(&udp);
static Gateway gateway1(&linkinterfaceUDP, true);

