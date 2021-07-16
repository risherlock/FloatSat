
#include "rodos.h"
#include "gateway.h"

static UDPInOut         udp(-50000);
static LinkinterfaceUDP linkinterfaceUDP(&udp);
static Gateway          gateway1(&linkinterfaceUDP, true);
