#include "rodos.h"
#include "subsCopyToObj-topics.h"

Position pp;
SubsCopyToObj<Position>  nameNotImportant04(&pp, position, "postionxx");

class ReceiverPos : public StaticThread<> {
    void run () {
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            PRINTF( "receiver %f %f %f\n", pp.x, pp.y, pp.z);
        }
    }
} recPos;

