set(port_dir "bare-metal/raspberrypi3")
set(is_port_baremetal TRUE)

set(RODOS_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(OSC_CLK 8000000)

add_compile_definitions(HSE_VALUE=${OSC_CLK} STM32F40_41xxx USE_STM32_DISCOVERY USE_STDPERIPH_DRIVER)

add_compile_options(-mfloat-abi=soft -mcpu=cortex-a53 -nostartfiles)
add_link_options(-T${RODOS_DIR}/src/bare-metal/raspberrypi3/scripts/linkerscript.ld)
add_link_options(--entry=_exception_table -Wl,--gc-sections -nodefaultlibs -nostdlib -ffreestanding -fno-strict-aliasing -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables)

set(sources_to_add
    ${RODOS_DIR}/src/bare-metal/raspberrypi3/startup/*.c
    ${RODOS_DIR}/src/bare-metal/raspberrypi3/startup/*.S)

set(libraries_to_link gcc)
