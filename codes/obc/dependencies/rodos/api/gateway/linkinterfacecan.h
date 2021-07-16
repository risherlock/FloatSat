#pragma once


#include <stdint.h>


#include "gateway/linkinterface.h"
#include "hal.h"
#include "hal/hal_can.h"


namespace RODOS {


constexpr uint8_t CAN_LINK_NODE_BITS = 8;
constexpr uint8_t CAN_LINK_TOPIC_BITS = 16;
constexpr uint32_t CAN_LINK_ID = (0x1C << (CAN_LINK_NODE_BITS + CAN_LINK_TOPIC_BITS));

//Number of CAN messages to buffer when multiple messages are interleaved during receiving
constexpr int32_t CAN_MSG_BUFFER_SIZE = 128;

//Timeout after which transmitting is canceld if no ack is received
constexpr int64_t CAN_TX_TIMEOUT = (50 * MILLISECONDS);


struct BufferedCANMessage {
	uint32_t canID;
	BufferedCANMessage* next;
	uint8_t data[8];
	int8_t len;
};



class LinkinterfaceCAN : public Linkinterface, IOEventReceiver {

	HAL_CAN& can;

	BufferedCANMessage BufferedCANMessage_memory[CAN_MSG_BUFFER_SIZE];
	BufferedCANMessage* emptyBufferedCANMessages[CAN_MSG_BUFFER_SIZE];
	int32_t emptyBufferedCANMessagesPos;
	BufferedCANMessage* bufferedCANMsgs_start;
	BufferedCANMessage* bufferedCANMsgs_end;

	NetworkMessage* currentMsg;
	uint32_t currentReceiveCANId;
	uint8_t* currentDataPointer;
	uint16_t bytesReceived;
	uint8_t sequenceCounter;

	BufferedCANMessage* receiveCANMessage();
	bool appendCANMsgToCurrentNetMsg(BufferedCANMessage* canMsg);


public:

    LinkinterfaceCAN(HAL_CAN* _can);

    virtual ~LinkinterfaceCAN() { }

    void init();

    virtual bool sendNetworkMsg(NetworkMessage& outMsg);
    bool getNetworkMsg(NetworkMessage &inMsg,int32_t &numberOfReceivedBytes);

    void onWriteFinished();
    void onDataReady();

    virtual void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME);

};

}  // namespace


