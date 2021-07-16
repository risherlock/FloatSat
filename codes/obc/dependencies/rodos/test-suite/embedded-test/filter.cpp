#include "rodos.h"

Topic<int> counter1(20, "counter1");
Topic<int> counter2(21, "counter2");

uint32_t printfMask = 0;

//Filter for counter1

class Filter : public TopicFilter {
  public:
    void prolog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        int* received = (int*)data;
        *received = *received + 1000000;
    }
    void epilog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, [[gnu::unused]] void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("Filter changed the value\n");
    }
} filter;



//Publisher for both topics
class Sender : public StaticThread<> {

    int c1 = 0;
    int c2 = 1000;

  public:
    void run() {
        printfMask = 1;
        for(int i = 0; i < 50; i++) {
            PRINTF("Publish cnt1 %d.\n", c1);
            counter1.publish(c1);
            c1++;

            PRINTF("Publish cnt2 %d.\n", c2);
            counter2.publish(c2);
            c2++;

            PRINTF("Publish cnt1 %d.\n", c1);
            counter1.publish(c1);
            c1++;
        }

        if (c1 > 50) {counter1.setTopicFilter(&filter);}
        
        for(int i = 0; i < 50; i++) {
            PRINTF("Publish cnt1 %d.\n", c1);
            counter1.publish(c1);
            c1++;

            PRINTF("Publish cnt2 %d.\n", c2);
            counter2.publish(c2);
            c2++;

            PRINTF("Publish cnt1 %d.\n", c1);
            counter1.publish(c1);
            c1++;
            
            
        }

        
        AT(500*MILLISECONDS);
        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        hwResetAndReboot();

    }
} sender;

//Subscriber1

class Receiver1 : public SubscriberReceiver<int> {
  public:
    Receiver1() : SubscriberReceiver<int>(counter1, "Subscriber1") {}
    void put(int& data) {
        int received = data;
        PRINTF("  receiver1-1 %d\n", received);
    }
} sub1;


//Subscriber2

class Receiver21 : public SubscriberReceiver<int> {
  public:
    Receiver21() : SubscriberReceiver<int>(counter2, "Subscriber2") {}
    void put(int& data) {
        int received = data;
        PRINTF("  receiver2-1 %d\n", received);
    }
} sub21;

class Receiver22 : public SubscriberReceiver<int> {
  public:
    Receiver22() : SubscriberReceiver<int>(counter2, "Subscriber2") {}
    void put(int& data) {
        int received = data;
        PRINTF("  receiver2-2 %d\n", received);
    }
} sub22;

