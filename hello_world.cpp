#include "rodos.h"

class HelloWorld : public StaticThread<>
{
  uint32_t count = 0;

  void run()
  {
    TIME_LOOP(100 * MILLISECONDS, 1000 * MILLISECONDS)
    {
      PRINTF("%ld: Hello World!\n", count++);
    }
  }
} hello_world;
