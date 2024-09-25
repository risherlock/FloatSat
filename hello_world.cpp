#include "rodos.h"

class HelloWorld : public StaticThread<>
{
  void run()
  {
    TIME_LOOP(100 * MILLISECONDS, 100 * MILLISECONDS)
    {
      PRINTF("Hello World!\n");
    }
  }
} hello_world;
