#include "rodos.h"
#include "common.h"

namespace APP02 {

Application application("APP02", 2);

class AppT: public StaticThread<> {
public:
    AppT() : StaticThread<>("AppT01:", 400)  { }
    void run() {
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            PRINTF("APP2:  counter1: %d\n", static_cast<int>(Common::counter1));
        } // loop
    }

} appT;

} // namesapce
