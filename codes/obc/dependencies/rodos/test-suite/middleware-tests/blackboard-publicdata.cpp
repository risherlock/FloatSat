#include "rodos.h"

uint32_t printfMask = 0;

struct SharedData {
    int  versionNr;
};


Topic<SharedData> deliverShared(-1, "deliverData");
Topic<SharedData> collectShared(-1, "CollectData");


/************************* Basis ***********************************/

struct BlackBoard {
    Semaphore protector;
    SharedData sharedData;
} blackBoard;

#define LK  blackBoard.protector.enter()
#define ULK blackBoard.protector.leave()
void deliverSharedCaller(SharedData &source)      { LK; blackBoard.sharedData = source;             ULK;}
void collectSharedCaller(SharedData &destination) { LK; destination = blackBoard.sharedData;        ULK;}
#undef LK
#undef ULK

SubscriberReceiver<SharedData> nameNotImportant01(deliverShared, deliverSharedCaller);
SubscriberReceiver<SharedData> nameNotImportant02(collectShared, collectSharedCaller);


/**********************************************************/

class Sender : public StaticThread<> {
public:
    SharedData myData;
    Sender() : StaticThread<>("Sender") { }
    void run () {
        printfMask = 1;
        myData.versionNr = 0;
        for (int i = 0; i < 10; i++) {
            PRINTF("Sending %d\n", myData.versionNr);
            deliverShared.publish(myData);
            myData.versionNr++;
            AT(NOW() + 100 * MILLISECONDS);
        }
        
        AT(2 * SECONDS);
        hwResetAndReboot();
    }
} sender;

class Receiver : public StaticThread<> {
public:
    SharedData myData;
    Receiver() : StaticThread<>("Receiver") { }
    void run () {
        printfMask = 1;
        AT(NOW() + 50 * MILLISECONDS);
        for (int i = 0; i < 5; i++) {
            collectShared.requestLocal(myData);
            PRINTF("    got %d\n", myData.versionNr);
            AT(NOW() + 300 * MILLISECONDS);
        }
    }
} receiver;


