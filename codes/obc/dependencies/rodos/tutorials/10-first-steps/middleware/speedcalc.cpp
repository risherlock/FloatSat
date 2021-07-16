#include "rodos.h"
#include "topics.h"
#include "math.h"

class SpeedCalc : public Subscriber {
  public:
    SpeedCalc() : Subscriber(position, "SpeedCalc") {}
    Pos  p0, p1;
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        p0       = p1;
        p1       = *static_cast<Pos*>(data);
        double v = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y) + (p0.z - p1.z) * (p0.z - p1.z));
        speed.publish(v);
        return 1;
    }
} speedCalc;
