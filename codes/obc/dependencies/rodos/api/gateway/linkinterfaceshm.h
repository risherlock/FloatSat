#pragma once


#include <stdint.h>


#include "fifo.h"
#include "gateway/networkmessage.h"
#include "gateway/linkinterface.h"
#include "hal/hal_sharedmemory.h"
#include "thread.h"

namespace RODOS {

constexpr size_t   FIFOSIZE   = 10;
constexpr uint32_t MAXMEMBERS = 50; //defines the maximum amount of participants in the fifo and shared memory


class LinkinterfaceSHM: public Linkinterface, IOEventReceiver {

protected:
	MultipleReaderFifo<NetworkMessage, FIFOSIZE, MAXMEMBERS> * fifo;
	int32_t readerId;
	Sharedmemory_IDX shmIdx;

public:

	LinkinterfaceSHM(Sharedmemory_IDX shmIdx);

	virtual ~LinkinterfaceSHM() {
	}

	/** To initialize the hardware and to establish physical interfaces.
	 * Depending on the type of interface set interrupt handlers (UART) or
	 * select a port (UDP) and init the local node number if possible.
	 */
	void init();

	/**
	 *Processs all received Data. When a NetworkMessage is complete it returns true
	 *@param inMsg NetworkMessage to write to. When return false this may contain incomplete Data, so do not touch it
	 *@param numberOfReceivedBytes When return=true this contains the number of Bytes really received or -1 if it is unknown.
	 *		Should be ==Networkmessage.numberOfBytesToSend() in case of no error.
	 */
	bool getNetworkMsg(NetworkMessage &inMsg, int32_t &numberOfReceivedBytes);

	/**
	 *  Sends NetworkMessage over the Link. May block. When it returns the Message should be send out or have been buffered.
	 * @param The Message to send
	 */
	bool sendNetworkMsg(NetworkMessage& outgoingMessage);

	void onWriteFinished();

	virtual void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME);
};

}  // namespace


