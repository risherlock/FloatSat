set(board discovery)
set(OSC_CLK 8000000)
set(RODOS_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")
set(linker_script ${RODOS_DIR}/src/bare-metal/stm32f4/scripts/stm32_flash.ld)

include(${CMAKE_CURRENT_LIST_DIR}/stm32f4.cmake)
