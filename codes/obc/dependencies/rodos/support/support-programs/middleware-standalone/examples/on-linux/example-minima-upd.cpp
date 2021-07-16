/*
 * main.cpp
 *
 *  Created on: 10.07.2013
 *      Author: Erik
 */


#include <stdlib.h>
#include <stdio.h>

#include "rodos-middleware.h"
#include <unistd.h>
#include <sys/time.h>


UDPInOut udp(-50000);  // the same used in on_rodos example
LinkinterfaceUDP linkif(&udp);
static Gateway gw(&linkif);

int64_t NOW() {
    struct timeval t;
    gettimeofday(&t,0);
    return (t.tv_sec*1000000LL+t.tv_usec)*1000;
}


class MessageHandler : public Putter {
    virtual bool putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo) {
        printf("Got topic %ld, counter %ld, len %d\n", topicId, *(long*)msg, len);
        return true;
    }

} msgHandler;


int main(){
        int topicId = 1002; // the same used in on_rodos example
	printf(" Rodos interface als standalone (no rodos)\n");
        long myCnt = 500;

	gw.init(3); // 3 ist my simulated nodeid
	gw.setPutter(&msgHandler); // ony one

	while(1){
		gw.pollMessages();

		gw.sendNetworkMessage((char*)&myCnt, sizeof(myCnt), topicId, NOW());
		printf("sending %ld\n", myCnt++);
		usleep(500000);

	}
        return 0;
}


