#include "rodos.h"

HAL_GPIO pin_green_led(GPIO_060);
HAL_GPIO pin_orange_led(GPIO_061);
HAL_GPIO pin_red_led(GPIO_062);
HAL_GPIO pin_blue_led(GPIO_063);

class HelloBlinky : public StaticThread<>
{
  bool flag;

  void init()
  {
    // LED config
    pin_green_led.init(true, 1, 0);
    pin_orange_led.init(true, 1, 0);
    pin_red_led.init(true, 1, 0);
    pin_blue_led.init(true, 1, 0);
  }

  void run()
  {
    TIME_LOOP(100 * MILLISECONDS, 100 * MILLISECONDS)
    {
      flag = (!flag);

      // Enable LEDs
      pin_green_led.setPins(flag);
      pin_orange_led.setPins(flag);
      pin_red_led.setPins(flag);
      pin_blue_led.setPins(flag);
    }
  }
} hello_blinky;
