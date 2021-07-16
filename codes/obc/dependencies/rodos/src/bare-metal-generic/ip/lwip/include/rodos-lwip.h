/*
 * rodos-lwip.h
 *
 *  Created on: 22.09.2014
 *      Author: Erik
 */

#ifndef RODOS_LWIP_H_
#define RODOS_LWIP_H_

#include "rodos.h"
#include "hal/hal_ethernet.h"


#define LINK_CHECK_INTERVAL (2*SECONDS)


struct IPAddr {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
};

class IPStack : StaticThread<> {
	void run();

public:
	HAL_ETH* eth;
	bool linkstatus,dhcp;
	IPAddr ip,mask,gw;
	bool ipstackInitialized;
	IPStack(HAL_ETH* _eth,IPAddr* _ip=0,IPAddr* _mask=0,IPAddr* _gw=0);
	Semaphore IPsem;
	static IPStack* instance;

	void ensureInit();
};


#endif /* RODOS_LWIP_H_ */
