
/** timer template **/

#include "rodos.h"
#include "hw_specific.h"

namespace RODOS {
int64_t Timer::microsecondsInterval = 0;

void Timer::init()  { }
void Timer::start() { }
void Timer::stop()  { }
void Timer::setInterval([[gnu::unused]] const int64_t microsecondsInterval) { }


}
