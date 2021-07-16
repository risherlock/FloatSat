#include "rodos.h"
#include "mytopics.h"


/******************************/

static Application Receiver("Subscriber pong", 2011);

class PingReceiver : public Subscriber {
  public:
    PingReceiver() : Subscriber(ping, "pingReceiver") {}
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        long cnt = *static_cast<long*>(data);
        pong.publish(cnt);
        PRINTF("got ping %ld \n", cnt);
        return 1;
    }
} pingreceiver;
