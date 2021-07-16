#include <rodos.h>

namespace RODOS {

char  getcharNoWait() { return 0; }
bool  isCharReady() { return false; }
char* getsNoWait() { return 0; }
void  activateTopicCharInput() {}

} // namespace RODOS


// The compiler uses this in this case: int a[xxx] = {};
extern "C" {
void *memset(void *s, int c, size_t n) {
  return RODOS::memset(s, static_cast<char>(c), n);
}
}
