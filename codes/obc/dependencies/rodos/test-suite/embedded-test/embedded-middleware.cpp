#include "rodos.h"

#define NUMBER_OF_TESTS 10      //Whenever a test runs in a loop, the upper limit of executions is this number

uint32_t printfMask = 0;

/* Unique Strings to find classes:
 * MainTest:            FIND_MainTest
 * MainTest run:        FIND_MainTest_run
 * Sender:              FIND_Middleware_Sender
 * Alice:               FIND_Alice
 * Bob:                 FIND_Bob
 * Charly:              FIND_Charly
 */

// FIND_MainTest 
class MainTest : public StaticThread<> {
    
public:
    
    MainTest() : StaticThread<>("MainTest") {}
    void run();
    
}maintest;


//Middleware-Section-----------------------------------------------------------
Topic<int> counter1(20, "counter1");
Topic<int> counter2(21, "counter2");

//Publisher for both topics
// FIND_Middleware_Sender
class Sender : public StaticThread<> {
    
    int c1 = 0;
    int c2 = 1000;
    
public:
    void run() {
        suspendCallerUntil();
        
        for(int i = 0; i < NUMBER_OF_TESTS; i++) {
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
        
        maintest.resume();
        suspendCallerUntil();
        
    }
} middlewaresender;

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


//Alice-Bob-Charly-------------------------------------------------------------

/**************************************************** Common Data ***/

struct Greetings {
    int64_t date;
    char    msg[80];
};

Topic<Greetings> valentine(22, "valentine");


/********************************************************* Charly  ***/

// FIND_Charly
class Charly : public TopicFilter {
public:
    void prolog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        Greetings* grts = (Greetings*)data;
        grts->msg[0]    = 'I'; // correction
    }
    void epilog([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, [[gnu::unused]] void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("charly knows, message was distributed\n");
    }
} charly;


/************************************************************ Bob ***/

// FIND_Bob
class Bob : public StaticThread<> {
    Greetings greetings;
    
public:
    void run() {
        suspendCallerUntil();
        
        greetings.date = 111;
        strcpy(greetings.msg, "i wish you would be here");
        PRINTF("Bob sending greetings '%s'\n", greetings.msg);
        valentine.publish(greetings);
        
        valentine.setTopicFilter(&charly);
        valentine.publish(greetings);
        
        AT(500*MILLISECONDS);
        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        
        maintest.resume();
    }
} bob;

/********************************************************* Alice  ***/

// FIND_Alice
class Alice : public SubscriberReceiver<Greetings> {
public:
    Alice() : SubscriberReceiver<Greetings>(valentine, "aliceSubs") {}
    void put(Greetings& data) {
        Greetings* greetingsForMe = (Greetings*)&data;
        PRINTF("Alice got date %lld:  msg: %s\n", static_cast<long long>(greetingsForMe->date), greetingsForMe->msg);
    }
} alice;



// FIND_MainTest_run
void MainTest::run() {
    printfMask = 1;
    PRINTF("MAINTEST\n");
    
    //Middleware
    PRINTF("Middleware------------------\n");
    middlewaresender.resume();
    suspendCallerUntil();
    
    //Alice-Bob-Charly
    PRINTF("Alice-Bob-Charly--------------\n");
    bob.resume();
    suspendCallerUntil();
    
    hwResetAndReboot();
}



