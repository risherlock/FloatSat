#include <new>
#include <stdint.h>


#include "gateway/linkinterfaceshm.h"
#include "hal/hal_sharedmemory.h"
#include "rodos-assert.h"

namespace RODOS {

constexpr int32_t FIFO_MAGIC_WORD = 1234;

HAL_Sharedmemory hal_sharedmemory(Sharedmemory_IDX0);

LinkinterfaceSHM::LinkinterfaceSHM(Sharedmemory_IDX shmIdx) :
		Linkinterface(-1) {
	this->shmIdx = shmIdx;
}

void LinkinterfaceSHM::init() {

	Linkinterface::init();
	void * sharedMemoryUserSpace;
	fifo=0;

	new (&hal_sharedmemory) HAL_Sharedmemory(shmIdx); // placement new to avoid dynamic memory allocation
	//get sharedMemory pointer
	size_t size = 20000; //20kB
	int32_t maxMembers = MAXMEMBERS;
	sharedMemoryUserSpace = hal_sharedmemory.init(&size, &maxMembers);
	this->readerId = hal_sharedmemory.getMemberId();

	RODOS_ASSERT_IFNOT_RETURN_VOID(sharedMemoryUserSpace != 0);

	hal_sharedmemory.setIoEventReceiver(this);

	//lock shared memory
	while (hal_sharedmemory.lock() == false)
		;
	//create fifo
	if (*((int32_t*) sharedMemoryUserSpace) == FIFO_MAGIC_WORD) {
		fifo = (MultipleReaderFifo<NetworkMessage, FIFOSIZE, MAXMEMBERS> *) ((int32_t*) sharedMemoryUserSpace + 1);
	} else {
		*((int32_t*) sharedMemoryUserSpace) = FIFO_MAGIC_WORD;
		fifo = new ((int32_t*) sharedMemoryUserSpace + 1) MultipleReaderFifo<NetworkMessage, FIFOSIZE, MAXMEMBERS>();
	}

	//unlock shared memory
	hal_sharedmemory.unlock();

}

bool LinkinterfaceSHM::sendNetworkMsg(NetworkMessage &outMsg) {
	if(fifo==0)
		return false;

	//lock shared memory
	while (hal_sharedmemory.lock() == false)
		;

	//write
	fifo->put(outMsg);

	//unlock
	hal_sharedmemory.unlock();

	//inform other processes
	hal_sharedmemory.raiseSharedMemoryChanged();

	return true;
}

bool LinkinterfaceSHM::getNetworkMsg(NetworkMessage &inMsg, int32_t &numberOfReceivedBytes) {
	if(fifo==0 || readerId < 0)
		return false;
	numberOfReceivedBytes = -1;
	hal_sharedmemory.resetDataReady();//ack shared memory
	return fifo->get(inMsg, static_cast<uint32_t>(readerId));
}

void LinkinterfaceSHM::onWriteFinished() {
    //Method does not make sense for HAL_Sharedmemory because there is no write function
}

//void LinkinterfaceSHM::onDataReady() {
//    if(threadToResume) threadToResume->resume();
//}
void LinkinterfaceSHM::suspendUntilDataReady(int64_t reactivationTime){
	//TODO check if FIFO is empty
	hal_sharedmemory.suspendUntilDataReady(reactivationTime);
}

}
