#include "rodos.h"
#include "topics.h"
#include "random.h"

class PositionSensor : public StaticThread<> {
  public:
    PositionSensor() : StaticThread<>("PositionSensor") {}

    void run() {
        Pos p = { 0.0, 0.0, 0.0 };
        TIME_LOOP(2 * SECONDS, 3 * SECONDS) {
            p.x += (uint32Rand() % 40) * 0.05 - 1;
            p.y += (uint32Rand() % 40) * 0.05 - 1;
            p.z += (uint32Rand() % 40) * 0.05 - 1;
            position.publish(p);
        }
    }
} positionSensor;
