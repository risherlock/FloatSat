#include "rodos.h"
#include "matlib.h"
#include "filter-topics.h"


class MyPublisher05 : public StaticThread<> {
    Vector3D vec;
    double   doub;
public:
    MyPublisher05() : StaticThread<>("send for filtering", 150) { }
    void run () {
        doub = 0;
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            doub += 10;
            vec   = vec + Vector3D(0.1, 0.2, 0.3);
            PRINTF("Sending %f, (%f %f %f)\n", doub, vec.x, vec.y, vec.z);
            topicDoub.publish(doub);
            topicVect.publish(vec);
        }
    }
} publisher05;

