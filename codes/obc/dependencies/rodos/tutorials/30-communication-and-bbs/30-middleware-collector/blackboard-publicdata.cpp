#include "rodos.h"

struct SharedData {
    char string[20];
    int  versionNr;
};


Topic<SharedData> deliverShared(-1, "deliverData");
Topic<SharedData> collectShared(-1, "CollectData");

Topic<int64_t> deliverVar01(-1, "deliverVar01");
Topic<int64_t> collectVar01(-1, "CollectVar01");

Topic<int64_t> deliverVar02(-1, "deliverVar02");
Topic<int64_t> collectVar02(-1, "CollectVar02");



/************************* Basis ***********************************/

struct BlackBoard {
    Semaphore protector;
    SharedData sharedData;
    int64_t onlyDemoNotUsed01;
    int64_t onlyDemoNotUsed02;
} blackBoard;

#define LK  blackBoard.protector.enter()
#define ULK blackBoard.protector.leave()
void deliverSharedCaller(SharedData &source)      { LK; blackBoard.sharedData = source;             ULK;}
void collectSharedCaller(SharedData &destination) { LK; destination = blackBoard.sharedData;        ULK;}

void deliverVar01Caller(int64_t &source)          { LK; blackBoard.onlyDemoNotUsed01 = source;      ULK;}
void collectVar01Caller(int64_t &destination)     { LK; destination = blackBoard.onlyDemoNotUsed01; ULK;}

void deliverVar02Caller(int64_t &source)          { LK; blackBoard.onlyDemoNotUsed02 = source;      ULK;}
void collectVar02Caller(int64_t &destination)     { LK; destination = blackBoard.onlyDemoNotUsed02; ULK;}
#undef LK
#undef ULK

SubscriberReceiver<SharedData> naneNotImportant01(deliverShared, deliverSharedCaller);
SubscriberReceiver<SharedData> naneNotImportant02(collectShared, collectSharedCaller);
SubscriberReceiver<int64_t>    naneNotImportant03(deliverVar01,  deliverVar01Caller);
SubscriberReceiver<int64_t>    naneNotImportant04(collectVar01,  collectVar01Caller);
SubscriberReceiver<int64_t>    naneNotImportant05(deliverVar02,  deliverVar02Caller);
SubscriberReceiver<int64_t>    naneNotImportant06(collectVar02,  collectVar02Caller);

/**********************************************************/

class Sender : public StaticThread<> {
public:
    SharedData myData;
    Sender() : StaticThread<>("Sender") { }
    void run () {
        myData.versionNr = 0;
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            PRINTF("Sending %d\n", myData.versionNr);
            xsprintf(myData.string, "Ver %d", myData.versionNr);
            deliverShared.publish(myData);
            myData.versionNr++;
        }
    }
} sender;

class Receiver : public StaticThread<> {
public:
    SharedData myData;
    Receiver() : StaticThread<>("Receiver") { }
    void run () {
        TIME_LOOP(3500*MILLISECONDS, 3*SECONDS) {
            collectShared.requestLocal(myData);
            PRINTF("    got %d : %s\n", myData.versionNr, myData.string);
        }
    }
} receiver;


