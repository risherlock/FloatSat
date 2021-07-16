if(NOT board)
  message(FATAL_ERROR "Do not use stm32f4 directly; instead, use a board port file.")
endif()

set(port_dir "bare-metal/stm32f4")
set(is_port_baremetal TRUE)

set(RODOS_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

if(NOT DEFINED OSC_CLK)
    set(OSC_CLK 8000000)
    MESSAGE("DID NOT GET OSC_CLK Override, using default: ${OSC_CLK}")
else()
    MESSAGE("OSC_CLK used ${OSC_CLK}")
endif()

if(NOT DEFINED linker_script)
    set(linker_script ${RODOS_DIR}/src/bare-metal/stm32f4/scripts/stm32_flash.ld)
    MESSAGE("DID NOT GET linker_script Override, using default: ${linker_script}")
else()
    MESSAGE("OSC_CLK used ${linker_script}")
endif()

add_compile_definitions(HSE_VALUE=${OSC_CLK} STM32F40_41xxx USE_STM32_DISCOVERY USE_STDPERIPH_DRIVER)

set(compile_and_link_options -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16)
add_compile_options(-gdwarf-2 -mthumb)
add_compile_options(${compile_and_link_options})
add_link_options(${compile_and_link_options})
add_link_options(-T${linker_script})
add_link_options(-nostartfiles -nodefaultlibs -nostdlib -Xlinker --gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables)

set(sources_to_add
    ${RODOS_DIR}/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/src/*.c
    ${RODOS_DIR}/src/bare-metal/stm32f4/startup/*.c
    ${RODOS_DIR}/src/bare-metal/stm32f4/startup/*.S)

set(directories_to_include
    ${RODOS_DIR}/src/bare-metal/stm32f4/STM32F4xx_StdPeriph_Driver/inc
    ${RODOS_DIR}/src/bare-metal/stm32f4/CMSIS/Device/ST/STM32F4xx/Include
    ${RODOS_DIR}/src/bare-metal/stm32f4/CMSIS/Include
    ${RODOS_DIR}/src/bare-metal/stm32f4/platform-parameter/${board})

set(libraries_to_link
    m
)
