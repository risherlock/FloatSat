#include "rodos.h"
#include "support-libs.h"

static Application applic("SyncComBufTest");

SyncCommBuffer<int> buf;

class Sender : public StaticThread<> {
   void run () {
    int  cnt = 0;
    while(1) {
      cnt++;
      PRINTF("Writing %d\n", cnt);
      buf.put(cnt);
      if(cnt % 3 == 0) {
          PRINTF("Writing Zero\n");
          buf.putZero();
      }
      suspendCallerUntil(NOW() + 3*SECONDS);
    }
  }
};

class Receiver : public StaticThread<> {
  void run () {
    int cnt;
    while(1) {
      bool newData = buf.syncGet(cnt, 2*SECONDS);
      if (newData){
        PRINTF("Reading %d\n", cnt);
      }else{
        PRINTF("No new data available\n");
      }
    }
  }
};

/******************************/

Sender   sender;
Receiver receiver;
