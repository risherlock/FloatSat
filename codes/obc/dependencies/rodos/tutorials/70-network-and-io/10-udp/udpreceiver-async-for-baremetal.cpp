/*
in run dann erst in.reopen aufrufen mit der portnummer  !!!!!
ipstack wird nicht initialisiert wenn
static UDPIn nameNotImportatn01(5002, &udpMessages);
verwendet wird (geht nur unter linux da er dort schn initialisiert ist.
auf bare-metal muss er erst noch mit nameNotImportatn01.reopen(port) initialisiert werden.
*/

#include "rodos.h"
#include "ip/lwip/include/rodos-lwip.h"

static Application  receiverName("UDPReceiverAsync");

Topic<GenericMsgRef> udpMessages(-1, "udpMessages");

/*
 * Initialise the lwip ipstack and assign IP address, subentmask and gateway for bare-metal
 */
HAL_ETH halEth(ETH_IDX0);
IPAddr ip={172,16,10,200};
IPAddr mask={255,255,255,0};
IPAddr gw={171,16,10,1};
IPStack ipstack(&halEth, &ip, &mask, &gw);


static UDPIn in2(5002, &udpMessages);
static UDPIn in3(5003, &udpMessages);

void MAIN() {
    PRINTF("\n\n to test please type in other terminal\n");
    PRINTF("    netcat -u localhost 5002\n");
    PRINTF("    netcat -u localhost 5003\n");
}


/******************************/

// Use this async receiver implementation on bare-metal !
class Udpreceiver :  public StaticThread<>, Subscriber {
public:
    Udpreceiver() :
    	StaticThread<>("udpReceiver- StaticThread<>)"),
    	Subscriber(udpMessages, "udpReceiver") { }

    void putFromInterrupt(const long topicId, const void* data, int len) {
        GenericMsgRef* msg = (GenericMsgRef*)data;
        msg->msgPtr[msg->msgLen] = 0;
        xprintf("\nAsync: len: %d message:\n%s\n", msg->msgLen, msg->msgPtr); // no PRINTF in interrupts (Sempahore)
    }

    void run() {
    	in2.reopen(5002); // has to be done once, as otherwise IP stack is not initialised on bare-metal
    }

} udpReceiver;


