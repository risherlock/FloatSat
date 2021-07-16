
#include "rodos.h"

uint32_t printfMask = 0;

/**************************************************** Common Data ***/

struct Greetings {
    int64_t date;
    char    msg[80];
};

Topic<Greetings> valentine(20, "valentine");


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
        printfMask = 1;
        greetings.date = 111;
        strcpy(greetings.msg, "i wish you would be here");
        PRINTF("Bob sending greetings '%s'\n", greetings.msg);
        valentine.publish(greetings);

        valentine.setTopicFilter(&charly);
        valentine.publish(greetings);

        AT(500*MILLISECONDS);
        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();
    }
} bob;

/********************************************************* Alice  ***/

class Alice : public SubscriberReceiver<Greetings> {
  public:
    Alice() : SubscriberReceiver<Greetings>(valentine, "aliceSubs") {}
    void put(Greetings& data) {
        Greetings* greetingsForMe = &data;
        PRINTF("Alice got date %lld:  msg: %s\n", static_cast<long long>(greetingsForMe->date), greetingsForMe->msg);
    }
} alice;
