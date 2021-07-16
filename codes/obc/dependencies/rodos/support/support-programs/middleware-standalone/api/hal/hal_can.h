/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Michael Ruffer
 */


#pragma once

#include <stdint.h>
#include "hal/genericIO.h"
#include "hal/hal_gpio.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


enum CAN_PARAMETER_TYPE {
	CAN_PARAMETER_BAUDRATE


};


enum CAN_STATUS_TYPE {
	CAN_STATUS_RX_LEVEL,
	CAN_STATUS_RX_ERROR,
	CAN_STATUS_TX_ERROR
};

enum CAN_IDX {
	CAN_IDX0 = 0,
	CAN_IDX1,
	CAN_IDX2,
	CAN_IDX3,
	CAN_IDX4,
	CAN_IDX5
};


class HW_HAL_CAN;


class HAL_CAN : public GenericIOInterface {



private:
	HW_HAL_CAN* context;


/* public functions */

public:
	HAL_CAN(CAN_IDX canIdx, GPIO_PIN rxPin = GPIO_INVALID , GPIO_PIN txPin = GPIO_INVALID);

	/* Initialization of uart interface: mode 8N1, no HW flow control*/
	int init(unsigned int baudrate);

	/* disable interface and set all its registers and pins to its reset state */
	void reset();

 	/* Configuration of uart interface AFTER initialization */
	int config(CAN_PARAMETER_TYPE type, int paramVal);

	int status(CAN_STATUS_TYPE type);

	//Called when all data has benn transmitted
	bool isWriteFinished();
	//Called when a message is ready to read
	bool isDataReady();


	/**
	 * Add a filter for incoming Messages. Only Message with pass a filter are received. Filters may be implemented in hardware an can be limited.
	 * Also one ID should only match with one filter.
	 * @param ID CAN ID (11bit for standard frame 29 bit for extended frame, always right justified)
	 * @param IDmask every bit that is 1 here is don't care (11bit for standard frame 29 bit for extended frame, always right justified)
	 * @param RTR frame?
	 * @retval true when filter was created, false if not(no more hw rescources?)
	 */
	bool addIncomingFilter(uint32_t ID, uint32_t IDMask=0, bool extID = true, bool rtr=false);




	/**Transmit CAN Message, non-blocking if there is space in outputbuffer
	 *@param sendBuf pointer to transmit dara
	 *@param len lenth of transmit data( max. 8 bytes)
	 *@param stdID CAN ID (11bit for standart frame 29 bit for extended frame, always right justified)
	 *@param rtr send RTR frame?
	 *@retval -1 on error 0 on success
	 */
	int write(const char* sendBuf, int len,uint32_t canID,  bool extID = true, bool rtr=false);
	/** Receives CAN Message, non-blocking
	 * @param recBuf dataBuffer, must have space for 8 bytes
	 * @param stdID CAN ID of received frame (11bit for standard frame 29 bit for extended frame, always right justified)
	 * @param rtr was it a rtr frame?
	 * @retval number of databytes or -1 if nothing was received
	 */
	int read(char* recBuf, uint32_t* canID=0, bool* isExtID = 0,bool* rtr=0);


	//TODO: void setDEfaultTransmitID(...) //dann kann auch write und read aus GenericIO implementiert werden.

	// read und write aus GenericIO nicht implementiert



};




#ifndef NO_RODOS_NAMESPACE
}
#endif


