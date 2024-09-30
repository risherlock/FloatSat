# FloatSat

Software templates for *FloatSat Praktikum* at *Julius-Maximilians-Universität Würzburg*.

## Table of contents

1. [Dependencies](#1-dependencies)
2. [Usage](#2-usage)
3. [Examples](#3-examples)
4. [Caution!](#4-caution)
    - [Virtual COM port configuration](#virtual-com-port-configuration)

## 1. Dependencies

1. GNU Arm Embedded Toolchain: ```gcc-arm-none-eabi```
	- Windows installer: https://developer.arm.com/downloads/-/gnu-rm
	- Linux installation command: ```apt install gcc-arm-none-eabi```
2. Open On-Chip Debugger: ```openocd```
	- Windows installer: https://gnutoolchains.com/arm-eabi/openocd/
	- Linux installation command: ```sudo apt-get install openocd```

### Note

1. For Windows, please add ```bin``` directory of both GNU Toolchain and OpenOCD to the PATH environment variable.
2. Make sure that the installation was successful simply by checking version using the commands ```arm-none-eabi-gcc --version``` and ```openocd --version```.

## 2. Usage

1. Select your operating system on Makefile by setting ```OS``` variable. Only Linux and Windows is supported as of now.
2. Clone (if not) and compile RODOS: ```make rodos```
3. Compile an example: ```make main=hello_blinky.cpp```
4. Compile and flash example: ```make flash main=hello_blinky.cpp```

## 3. Examples

Each example is a RODOS thread provided to help you get started with RODOS and some modules for FloatSat.

1. [hello_blinky.cpp](hello_blinky.cpp) - Flashes four LEDs onboard the Discovery board.
3. [hello_world.cpp](hello_world.cpp) - Continuously prints ```Hello World!\n```  via UART port.
4. [hello_topic.cpp](hello_topic.cpp) - Inter-thread communication between two threads using a topic. Transmitter thread publishes the topic with increment of values every 5 seconds and the receiver thread subscribes to the topic once a second.

## 4. Caution!

### Virtual COM port configuration

In the Disovery boards provided in the lab, virtual COM ports has been enabled. This allows us to print messages using UART protocol via STLINK USB port onboard the STM32. PD8 and PD9 (UART3) pins are connected to the STLINK for the virtual COM.

However, RODOS configures UART2 by default. Therefore we should configure the appropriate UART port by uncommenting the corresponding ```UART_DEBUG``` in ```platform-parameter.h``` for the target device.

```C
#define UART_DEBUG UART_IDX3, GPIO_056, GPIO_057
```

For STM32F407 Discovery board, the file is in the directory ```rodos/src/bare-metal/stm32f4/platform-parameter/discovery```. Once RODOS is cloned, you can access the file via [this link](rodos/src/bare-metal/stm32f4/platform-parameter/discovery/platform-parameter.h).

Do not forget to recompile RODOS with ```make rodos``` after the configuration.
