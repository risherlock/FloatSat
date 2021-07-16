#include "rodos.h"
#include "topics.h"

static Application reciverSimple("ReciverSimple", 1100);

class SimpleSub : public Subscriber {
  public:
    SimpleSub() : Subscriber(counter1, "simplesub") {}
    uint32_t put(const uint32_t topicId, const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("SimpleSub - Length: %lu Data: %ld TopicId: %lu \n",
          static_cast<unsigned long>(len),
          *static_cast<long*>(data),
          static_cast<unsigned long>(topicId));
        return 1;
    }
} simpleSub; // try this: , a, b, c;
