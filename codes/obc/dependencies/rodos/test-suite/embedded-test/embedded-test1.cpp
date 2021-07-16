#include "rodos.h"
#include "checksumes.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

#define NUMBER_OF_TESTS 10      //Whenever a test runs in a loop, the upper limit of executions is this number

/* Unique Strings to find classes:
 * MainTest:            FIND_MainTest
 * MainTest run:        FIND_MainTest_run
 * CommBufferReceiver:  FIND_CommBufferReceiver
 * CommBufferSender:    FIND_CommBufferSender
 * FiFoSender:          FIND_FiFoSender
 * FiFo-Receiver:       FIND_FiFoReceiver
 * Printf-Test:         FIND_TestPrint
 * TestWaiter:          FIND_Resume_TestWaiter
 * TimeEventTest:       FIND_Resume_TestTimeEvent
 * Checksum:            FIND_Checksum
 */

// FIND_MainTest 
class MainTest : public StaticThread<> {
    
public:
    
    MainTest() : StaticThread<>("MainTest") {}
    void run();
    
}maintest;

//CommBuffer-Test section------------------------------------------------------

// Sender sends data which is then received by a different thread

CommBuffer<int> combufTest;


// FIND_CommBufferReceiver
class CommBufferReceiver : public StaticThread<> {
public:
    void run() {
        suspendCallerUntil();
        
        int mypos;
        suspendCallerUntil(NOW() + 10 * MILLISECONDS);
        while(1) {
            combufTest.get(mypos);
            PRINTF("Reading %d\n", mypos);
            suspendCallerUntil(NOW() + 30 * MILLISECONDS);
            if (mypos == (NUMBER_OF_TESTS - 1)) {
                suspendCallerUntil();
            }
        }
    }
}cbReceiver;


// FIND_CommBufferSender
class CommBufferSender : public StaticThread<> {
    
public:
    
    CommBufferSender() : StaticThread<>("CommBufferSender") {}
    
    void run() {
        suspendCallerUntil();
        cbReceiver.resume();
        
        for (int i = 0; i < NUMBER_OF_TESTS; i++) {
            PRINTF("Writing %d \n", i);
            combufTest.put(i);
            suspendCallerUntil(NOW() + 30 * MILLISECONDS);
        }
        
        maintest.resume();
        suspendCallerUntil();
    }
}cbSender;

//FiFo-Test-section------------------------------------------------------------
Fifo<int, NUMBER_OF_TESTS/2> fifo;

// FIND_FiFoReceiver
class FiFoReceiver : public StaticThread<> {
    
public:
    
    FiFoReceiver() : StaticThread<>("FiFoReceiver") {}
    
    void run() {
        suspendCallerUntil();
        
        int cnt;
        PRINTF("FiFo-Receiver\n");
        
        while(1) {
            bool ok = fifo.get(cnt);
            if(ok) {
                PRINTF("reading %d\n", cnt);
                if (cnt == (NUMBER_OF_TESTS - 1)) {
                    suspendCallerUntil();
                }
            } else {
                suspendCallerUntil(NOW() + 10 * MILLISECONDS);
            }
        }
    }
}fiforeceiver;


// FIND_FiFoSender
class FiFoSender : public StaticThread<> {
    
public:
    
    FiFoSender() : StaticThread<>("FiFo-Sender") {}
    
    void run() {
        suspendCallerUntil();
        fiforeceiver.resume();
        
        PRINTF("FiFo-Sender\n");
        for (int i = 0; i < NUMBER_OF_TESTS; i++) {
            
            bool ok = fifo.put(i);
            if(ok) {
                PRINTF("Sending %d\n", i);
            } else {
                PRINTF("FiFo full\n");
            }
            suspendCallerUntil(NOW() + 10 * MILLISECONDS);
            if((i % 5) == 0) {
                PRINTF("Waiting 0.03 seconds\n");
                suspendCallerUntil(NOW() + 30 * MILLISECONDS);
            }
        }
        
        maintest.resume();
        suspendCallerUntil();
    }
}fifosender;


//PRINTF specifier test section------------------------------------------------

// FIND_TestPrint
class TestPrint : public StaticThread<> {
public:
    void run() {
        suspendCallerUntil();
        
        PRINTF(" Showing different printf formats\n\n\n");
        
        double  a = 123456.789;
        double  b = -0.00000000123456789;
        double c;
        *(int64_t*)&c = 0x7FF8000000000000; // nan
        
        PRINTF(" as f : %f     %3.15f    %f\n", a, b, c);
        PRINTF(" as e : %e     %2.5e     %e\n", a, b, c);
        
        PRINTF("\n\n");
        PRINTF("as d:  %d\n", -12);
        PRINTF("as u:  %u\n", -12);
        PRINTF("as o:  %o\n", 12);
        PRINTF("as x:  %04x\n", 12);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
        PRINTF("as b:  %08b\n", 0x55); // please ignore the c++ compile warning
#pragma GCC diagnostic pop        
        
        PRINTF("\n\n");
        PRINTF("as s: '%s'\n", "this is a string");
        
        maintest.resume();
        suspendCallerUntil();
    }
    
} testPrint;


//FIND_Checksum

//Calculate the checksums of given data

//checkSumXor32 is identical with a given 32bit integer

union Data {
    uint32_t toBeTested;
    uint8_t data[4];
};

class ChecksumTester : public StaticThread<> {

  public:
    void run() {
        suspendCallerUntil();
        Data data;
        data.toBeTested = 1;
        char buff[80];          //Buffer for printf, a lot longer than needed
        
        CRC crc;
        
        
        for(uint32_t i = 1; i < 100; i++) {
            data.toBeTested *= i;
            data.toBeTested += i;
            SPRINTF(buff, "%u", static_cast<unsigned int>(data.toBeTested));
            PRINTF("%u Checksum of %u: %u ---- %u ---- %u ---- %u ---- %x\n",
                static_cast<unsigned int>(i),
                static_cast<unsigned int>(data.toBeTested),
                static_cast<unsigned int>(checkSum(data.data, 4)),
                static_cast<unsigned int>(computeCrc(data.data, 4, 0)),
                static_cast<unsigned int>(crc.computeCRC(data.data, 4, 0)),
                static_cast<unsigned int>(checkSumXor32(&data.toBeTested, 1)),
                hash(buff));
        }

        maintest.resume();
        suspendCallerUntil();

    }
} checksumsTester;


//Resume from event section----------------------------------------------------

// FIND_Resume_TestWaiter
class TestWaiter : public StaticThread<> {
public:
    void run() {
        suspendCallerUntil();
        
        for (int i = 0; i < NUMBER_OF_TESTS; i++) {
            PRINTF("Suspend and wait until someone resumes me\n");
            suspendCallerUntil();
            PRINTF("testwaiter running again at %3.9f\n", CNT_SECONDS_NOW());
        }
        
        maintest.resume();
        suspendCallerUntil();
    }
};


static TestWaiter testWaiter;

// FIND_Resume_TestTimeEvent
class TimeEventTest : public TimeEvent {
public:
    void handle() {
        xprintf("   Time Event at %3.9f\n", CNT_SECONDS_NOW());
        testWaiter.resume();
        xprintf("   Testwaiter resumed from me\n");
    }
    
    void init() {activatePeriodic(50 * MILLISECONDS, 30 * MILLISECONDS); }
};

static TimeEventTest te01;

// FIND_MainTest_run
void MainTest::run() {
    printfMask = 1;
    PRINTF("MAINTEST\n");
    
    //CommBuffer
    PRINTF("CommBuffer--------------------\n");
    cbSender.resume();
    suspendCallerUntil();
    
    //FiFo
    PRINTF("FiFo--------------------------\n");
    fifosender.resume();
    suspendCallerUntil();
    
    //Printf
    PRINTF("PRINTF------------------------\n");
    testPrint.resume();
    suspendCallerUntil();
    
    //Checksum
    PRINTF("Checksum----------------------\n");
    checksumsTester.resume();
    suspendCallerUntil();
    
    //TimeEvent, last test in this file as the TimeEvent can't be stopped
    PRINTF("TimeEvent---------------------\n");
    suspendCallerUntil();
    
    hwResetAndReboot();
}


