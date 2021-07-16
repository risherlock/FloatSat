#include "rodos.h"

uint32_t printfMask = 0;

/* Unique Strings to find classes:
 * MainTest:            FIND_MainTest
 * MainTest run:        FIND_MainTest_run
 * Semaphore:           FIND_Semaphore
 * Receiver-Subscriber: FIND_Receiver_Subscriber
 * Receiver-Putter:     FIND_Putter
 * FiFo-Receiver:       FIND_FiFo
 * Commbuffer:          FIND_receiverBuf
 */

// FIND_MainTest 
class MainTest : public StaticThread<> {
    
public:
    
    MainTest() : StaticThread<>("MainTest") {}
    void run();
    
}maintest;


// FIND_Semaphore
#define NUMBER_OF_THREADS   30
#define COUNT_TO            1000

#define USE_SEMA            //If not defined, no semaphores are used

Semaphore sema;
int tmp = 0;

class Thread1 : public StaticThread<> {
    
public:
    void run() {
        suspendCallerUntil();
        
        for(int i = 0; i < COUNT_TO; i++) {
            //AT(NOW() + 10 * MICROSECONDS);          //MUST be very short so the total time is not too long
            
            #ifdef USE_SEMA
            sema.enter();
            #endif            
            
            tmp++;
            
            #ifdef USE_SEMA
            sema.leave();
            #endif
        }
        
        #ifdef USE_SEMA
        if (tmp == (NUMBER_OF_THREADS * COUNT_TO)) {        //To make sure, only the last thread to finish prints its result, otherwise it might be not deterministic
            PRINTF("Result: %d (Correct)\n", tmp);
            maintest.resume();
        }
        #endif
        
        #ifndef USE_SEMA
        
        PRINTF("Result: %d \n", tmp);
        
        #endif
       
        
    }
} sema_test[NUMBER_OF_THREADS];


// FIND_Receiver_Subscriber

Topic<long> counter1(-1, "counter1");

static Application senderName("Publisher 01 simple", 1100);

class MyPublisher01 : public StaticThread<> {
  public:
    MyPublisher01() : StaticThread<>("SenderSimple") {}

    void run() {
        suspendCallerUntil();
        
        long cnt = 0;
        TIME_LOOP(30 * MILLISECONDS, 30 * MILLISECONDS) {
            if (cnt > 10) {
                maintest.resume();
            }
            PRINTF("Publisher01 sending Counter1 %ld\n", ++cnt);
            counter1.publish(cnt);
        }
    }
} subscriber_receiver;
 

static Application receiverSimple("ReceiverSimple", 1100);

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
} simpleSub;

//FIND_Putter

//Topic<long> counter1(-1, "counter1");
 

static Application receiverName("ReciverPutter", 1100);

class JustPrint : public Putter {

    /// Implements the complete generic interface of putter
    bool putGeneric(const uint32_t topicId, size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("ReceiverPutter -  msgLen: %lu data: %ld topicId: %lu\n",
            static_cast<unsigned long>(msgLen),
            *static_cast<const long*>(msg),
            static_cast<unsigned long>(topicId));
        return true;
    }

} justPrint;

static Subscriber nameNotImportant01(counter1, justPrint, "justprint01");


//FIND_FiFo
static SyncFifo<long, 5> fifo;
static Subscriber        nameNotImportant02(counter1, fifo, "fifo");

class ReceiverSync : public StaticThread<> {
    void run() {
        long cnt = 0;
        while(1) {
            fifo.syncGet(cnt);
            PRINTF("ReceiverSync - counter1: %ld\n", cnt);
        }
    }
} receiverSync;


//FIND_receiverBuf
static CommBuffer<long> buf;
static Subscriber       receiverBuf(counter1, buf, "receiverbuf");

class ReceiverBuf : public StaticThread<> {
    void run() {
        long cnt;
        suspendCallerUntil();
        TIME_LOOP(0, 30 * MILLISECONDS) {
            buf.get(cnt);
            PRINTF("ReceiverComBuffer - counter1: %ld\n", cnt);
        }
    }
} recbuf;

// FIND_MainTest_run
void MainTest::run() {
    printfMask = 1;
    //Semaphore-Test
    PRINTF("Semaphore-Test----------------\n");
    
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        sema_test[i].resume();
    }
    
    suspendCallerUntil();
    
    //Various Receivers
    PRINTF("Various Receivers-------------\n");
    subscriber_receiver.resume();
    recbuf.resume();
    suspendCallerUntil();
    
    suspendCallerUntil(NOW() + 500*MILLISECONDS);        //To make sure all PRINTFs are finished
    PRINTF("FINISHED----------------------\n");
    
    hwResetAndReboot();
}


 
