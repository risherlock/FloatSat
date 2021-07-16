#include "rodos.h"

Topic<char[20]>   	collectData(-1, "CollectData");

/******************************/

class Collector : public StaticThread<> {
public:
    Collector() : StaticThread<>("Collector") { }

    void run () {
        char buf[20];
        TIME_LOOP(1*SECONDS, 2*SECONDS) {
            PRINTF("Colleting data\n");
            collectData.requestLocal(buf);
            printCharsHex("Collected:", buf, 20);
        }
    }
} collector;


class FillData : public Subscriber {
public:
    FillData() :  Subscriber(collectData, "fillData") { }
        uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const uint32_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
		char* toWrite = static_cast<char*>(msg);
		static int cnt = 0;
		for(int i = 0; i < 20; i++) toWrite[i] = static_cast<char>((cnt*0x10 + i) & 0xFF);
		cnt++;
		return 1;
	}
} fillData;

