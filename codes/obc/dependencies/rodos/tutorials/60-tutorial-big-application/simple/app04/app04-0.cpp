#include "rodos.h"
#include "common.h"

namespace APP04 {

Application application("APP04", 4);

class AppT0: public StaticThread<> {
public:
    AppT0() : StaticThread<>("AppT04-0:", 400)  { }
    void run() {
        TIME_LOOP(1100*MILLISECONDS, 1*SECONDS) {
            PRINTF("APP4-0:\n");
        } // loop
    }

} appT0;

} // namesapce
