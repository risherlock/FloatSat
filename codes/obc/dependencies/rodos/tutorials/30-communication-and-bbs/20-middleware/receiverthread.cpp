#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("ReciverThread", 1500);

struct ReceiverThread :  public Subscriber,  public StaticThread<>  {

  CommBuffer<long> inputMsgBuffer;

  ReceiverThread() : Subscriber(counter1, "receiverthread") { }

  uint32_t put(const uint32_t topicId, const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
      PRINTF(PL "%lu" PT "%lu"  PD "%ld\n",
        static_cast<unsigned long>(len),
        static_cast<unsigned long>(topicId),
        *static_cast<long*>(data));
      inputMsgBuffer.put(*(long*)data);
      this->resume();                         // not to publish from interrupt, call a thread to do it
      return 1;
   }           

  void run () {
    long cnt = 0;
    while(1) {
      suspendCallerUntil(); // the subscriver sahll reactivate it 
      inputMsgBuffer.get(cnt);
      PRINTF(RD "%ld\n", cnt);
    }
  }
} receiverThread;

