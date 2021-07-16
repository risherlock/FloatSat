/*
 * Copyright (c) 2014 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Erik Dilger
 */


#pragma once

#include <stdint.h>
#include "hal/genericIO.h"

//#include "lwip/pbuf.h"
struct pbuf;

namespace RODOS {


enum ETH_PARAMETER_TYPE {
	ETH_PARAMETER_MACADDR

};

enum ETH_STATUS_TYPE {
	ETH_STATUS_RX_ERROR,
	ETH_STATUS_TX_ERROR
};

enum ETH_IDX {
	ETH_IDX0 = 0,
	ETH_IDX1,
	ETH_IDX2
};

enum ETH_ERR{
	ETH_ERR_OK = 0,
	ETH_ERR_BUFFERFULL,
	ETH_ERR_LINKDOWN,
	ETH_ERR_NODATA,
	ETH_ERR_FRAMEINVALID,
	ETH_ERR_UNKNOWN
};

class HW_HAL_ETH;

class HAL_ETH : public GenericIOInterface {
private:
	HW_HAL_ETH* context;


/* public functions */

public:
	HAL_ETH(ETH_IDX ethIdx);

	/* Reset and Initialization of Ethernet interface: called once*/
	int32_t init();
	/**
	 * Check if the link is up/down, Called regularly
	 * @retval True if link is up, false otherwise
	 */
	bool checkLinkStatus();
 	/* Configuration of ethernet interface AFTER initialization */
	int32_t config(ETH_PARAMETER_TYPE type, void* paramVal);

	int32_t status(ETH_STATUS_TYPE type);

	const uint8_t* getMACAddr();

	//True when all data has been transmitted
	// Note: upCallWriteFinished should be called for every frame transmitted
	bool isWriteFinished();
	//True when a frame is ready to read
	// Note: upCallDataReady should be called for every frame received
	bool isDataReady();

	/**Transmit Ethernet Frame, non-blocking
	 *@param writeBuffer pbuf that containes the frame, size and aligment unspecified, maybe a chain of multiple pbufs.
	 * 			If the HAL needs the buf after this function returns it should ref it and then free it when its done with it. The HAL may not call allocation/free functions in ISR.
	 *@retval ETH_ERR_OK on success, ETH_ERR_BUFFERFULL when buffers are full,ETH_ERR_LINKDOWN when the link is down
	 */
	ETH_ERR write(pbuf* writeBuffer);
	/** Receives Ethernet frame, non-blocking
	 * @param readBuffer pointer to pbuf*, gets assigned the new frame or 0 if no new frame.
	 * 			The HAL must allocate these pbufs, they get freed by the system. The HAL may not call allocation/free functions in ISR.
	 * @retval ETH_ERR_OK on success, ETH_NODATA if no new frame, ETH_FRAMEINVALID if frame is not vaild (e.g. CRC check failed), the driver may also drop such frames without any report
	 */
	ETH_ERR read(pbuf** readBuffer);
};


}


