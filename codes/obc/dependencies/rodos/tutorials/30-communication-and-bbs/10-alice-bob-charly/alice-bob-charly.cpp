
#include "rodos.h"

/**************************************************** Common Data ***/

struct Greetings {
    int64_t date;
    char    msg[80];
};

Topic<Greetings> valentine(20, "valentine");

#define VALENTINE_DAY	2*SECONDS
#define VALENTINE_DAY2	4*SECONDS

/********************************************************* Charly  ***/

class Charly : public TopicFilter {
  public:
    void prolog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        Greetings* grts = static_cast<Greetings*>(data);
        grts->msg[0]    = 'I'; // correction
    }
    void epilog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, [[gnu::unused]] void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("charly knows, message was distributed\n");
    }
} charly;


/************************************************************ Bob ***/

class Bob : public StaticThread<> {
    Greetings greetings;

  public:
    void run() {
        AT(VALENTINE_DAY);
        greetings.date = NOW();
        strcpy(greetings.msg, "i wish you would be here");
        PRINTF("Bob sending greetings at %lld '%s'\n", static_cast<long long>(NOW()), greetings.msg);
        valentine.publish(greetings);

        AT(VALENTINE_DAY2);
        valentine.setTopicFilter(&charly);
        valentine.publish(greetings);
    }
} bob;

/********************************************************* Alice  ***/

class Alice : public SubscriberReceiver<Greetings> {
  public:
    Alice() : SubscriberReceiver<Greetings>(valentine, "aliceSubs") {}
    void put(Greetings& data) {
        Greetings* greetingsForMe = (Greetings*)&data;
        PRINTF("Alice got date %lld:  msg: %s\n", static_cast<long long>(greetingsForMe->date), greetingsForMe->msg);
    }
} alice;
