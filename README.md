# FloatSat

Templates for *FloatSat Praktikum*.

## Dependencies

1. GNU Arm Embedded Toolchain: ```gcc-arm-none-eabi```
2. Open On-Chip Debugger: ```openocd```

## Usage

1. Select your operating system on Makefile by setting ```OS``` variable. Only Linux and Windows is supported as of now.
2. Clone (if not) and compile RODOS: ```make rodos```
3. Compile an example: ```make main=hello_blinky.cpp```
4. Compile and flash example: ```make flash main=hello_blinky.cpp```

## Examples

Each example is a RODOS thread provided to help you get started with RODOS and some modules for FloatSat.

1. [hello_blinky.cpp](hello_blinky.cpp) - Flashes four LEDs onboard the Discovery board.
3. [hello_world.cpp](hello_world.cpp) - Continuously prints ```Hello World!\n```  via UART port.

## Caution !

1. Make sure to configure the appropriate UART port by uncommenting the corresponding ```UART_DEBUG``` in platform-parameter.h for the target device. For STM32F407 Discovery board, the file is in the directory ```rodos/src/platform-parameter/discovery```.
