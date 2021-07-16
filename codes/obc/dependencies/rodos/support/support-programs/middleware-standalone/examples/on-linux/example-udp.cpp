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

//HAL_UART uart_stdout(UART_IDX2);

UDPInOut udp(-5000);
LinkinterfaceUDP linkif(&udp);
static Gateway gw(&linkif);

struct Position {
	char someData[3];
	long cntr;
	float x, y, z;
} __attribute__((packed));


int32_t timer=0;


int64_t NOW(){
	struct timeval t;
	gettimeofday(&t,0);
	return (t.tv_sec*1000000LL+t.tv_usec)*1000;
}



class MessageHandler : public Putter {


	virtual bool putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo) {

		Position* p;


		if(topicId==20840){



			p=(Position*)msg;

			if(p->cntr> timer){
				timer=p->cntr;

			}


			printf("Got clock len=%d data=%d timer=%d\n",len,p->cntr,timer);

		}else{
			//printf("Got other len=%d topicID=%d\n",len,topicId);
		}
		return true;
	}




}msgHandler;

int64_t fakeTimer;


int main(){

	Position outMsg;




	printf("Hello World!\n");

	gw.init(3);
	gw.setPutter(&msgHandler);



	while(1){


		//for(int i=0;i<100000;i++){
			gw.pollMessages();
		//}

		outMsg.cntr=timer;

		gw.sendNetworkMessage((char*)&outMsg,sizeof(outMsg),20840,NOW());

		printf("Sended\n");

		usleep(500000);


	}












return 0;
}


