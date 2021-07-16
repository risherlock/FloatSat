
#include "rodos.h"
#include "serializer-test.hh"
#include "stdio.h"

char downlinkBuffer[600];

Topic<char[600]>  tm(-1, "telemety");


SerializerTest orig;

//____________________________________________________________________

class TestController : public Thread {
    SerializerTest st;
    Serializable *stp = &st; // just to test virutal call
public:
  void run(){

    TIME_LOOP(1*SECONDS, 1*SECONDS) {

      st.update();
      orig = st;

      //int len = st.serialize(downlinkBuffer);
      int len = stp->serialize(downlinkBuffer);

      PRINTF("\nSending TM  %3.9f %d bytes\n", SECONDS_NOW(), len);
      
      PRINTF("\n SENDING: \n\n");
      PRINTF("i64 %lld i32 %d i16 %d i8 %d\n", st.i64, st.i32, st.i16,st.i8);
      PRINTF("ui64 %llu ui32 %u ui16 %u ui8 %u\n",st.ui64,st.ui32, st.ui16, st.ui8);
      PRINTF("d %f f %f b %d\n",st.d,st.f,st.b);
      PRINTF("[%d, %d, %d, %d]\n",st.a[0],st.a[1],st.a[2],st.a[3]);
      tm.publish(downlinkBuffer, false); // only local

    }
  }

} landerController;

//____________________________________________________________________


class GroundStation :  public Subscriber {
  SerializerTest st;
  Serializable *stp = &st;

public:
  GroundStation() : Subscriber(tm, "groundstation") { }

  long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
    // st.deserialize((char*)data);
    stp->deserialize((char*)data);
    PRINTF("\n GOT: \n\n");
    PRINTF("i64 %lld i32 %d i16 %d i8 %d\n", st.i64, st.i32, st.i16,st.i8);
    PRINTF("ui64 %llu ui32 %u ui16 %u ui8 %u\n",st.ui64,st.ui32, st.ui16, st.ui8);
    PRINTF("d %f f %f b %d\n",st.d,st.f,st.b);
    PRINTF("[%d, %d, %d, %d]\n",st.a[0],st.a[1],st.a[2],st.a[3]);

    // st.i32++; // just to generate an error messace to check the check is ok
    if(memcmp(&orig, &st, sizeof(SerializerTest))) PRINTF("\n********** ERRR***********\n\n");
    return 1;
  }
} groundStation;

