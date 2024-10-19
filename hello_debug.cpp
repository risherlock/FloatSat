#include "rodos.h"

class HelloDebug : public StaticThread<>
{
  uint16_t n = 17;
  uint16_t first_number = 1;
  uint16_t second_number = 1;
  uint16_t fibonacci = 0;

  void init()
  {
    for (uint16_t i = 2; i < n; i++)
    {
      fibonacci = first_number + second_number;
      first_number = second_number;
      second_number = fibonacci;
    }
  }

  void run()
  {
  }
} hello_debug;
