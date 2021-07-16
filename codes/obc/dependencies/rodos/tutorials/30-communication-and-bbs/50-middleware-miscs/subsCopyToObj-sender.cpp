#include "rodos.h"
#include "subsCopyToObj-topics.h"

static Position myPos;

class MyPublisher04 : public StaticThread<> {
public:
    MyPublisher04() : StaticThread<>("SenderPosition", 150) { }
    void run () {
        int32_t cnt = 100;
        TIME_LOOP(3500*MILLISECONDS, 3*SECONDS) {
            PRINTF("Sending %d...\n", static_cast<int>(cnt));
            myPos.x = cnt++;
            myPos.y = cnt++;
            myPos.z = cnt++;
            position.publish(myPos);
        }
    }
} publisher04;

