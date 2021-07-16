
#include "rodos.h"
#include "application-telemetry.hh"

char downlinkBuffer[600];

Topic<char[600]>  tm(-1, "telemety");

//____________________________________________________________________

class LanderController : public Thread {
  LanderTelemetry     lanTM;
public:
  void run(){
    lanTM.legContact[0] = lanTM.legContact[1] = lanTM.legContact[2] = lanTM.legContact[3] = false;
    lanTM.descentSpeed = 1.0;

    TIME_LOOP(1*SECONDS, 1*SECONDS) {

      lanTM.timeNow       = NOW();
      lanTM.descentSpeed /= 2.0;
      if(lanTM.descentSpeed < 0.001) lanTM.setLanded();

      int len = lanTM.serialize(downlinkBuffer);
      PRINTF("\nSending TM  %3.9f %d bytes\n", SECONDS_NOW(), len);
      tm.publish(downlinkBuffer, false); // only local

    }
  }

} landerController;

//____________________________________________________________________


class GroundStation :  public Subscriber {
  LanderTelemetry report;
public:
  GroundStation() : Subscriber(tm, "groundstation") { }

  long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
    report.deserialize((char*)data);
    PRINTF("   groundstation got time = %5.3f, speed = %f, contact = %d %d %d %d\n",
        (double)report.timeNow/SECONDS, report.descentSpeed,
        report.legContact[0], report.legContact[1], report.legContact[2], report.legContact[3]);
    return 1;
  }
} groundStation;

