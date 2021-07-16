#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019


if [ -z $TARGET_LIB ]; then
  cat << EOT1
  ****************
  *** Do not use this architecture directly, please use
  *** one of its derivates:
  ***    skith
  ***    discovery
  *********************
EOT1
exit 
fi



export ARCH=stm32f4         #used to select compile directories

#  From here, we can have different sub achictures. this is for stm34f40x, discovery
#  other possibilites: 
#     stm32f427
#     MINI-M4
#     stm32f429(STM32F429Discovery) 
#     stm32f40x (STM32F4Discovery),
#     stm32f401 (STM32F401 NUCLEO)
#
# We user STM32F407VGT6  (168MHz, 192KByte SRAM) == Discovery board


export SUB_ARCH=stm32f40x  # STM32F4Discovery, MINI-M4
#export OSC_CLK=8000000     # set in STM32F4Discovery, STM32F429Discovery
#export OSC_CLK=16000000     # set in SKITH

export ST_LINK="v2_1" #stm32f40x
#export ST_LINK="v2_0" #stm32f401

#---------------------------------------------- from here the same for all sub architectures

RODOS_ARCH_SRC1="${RODOS_SRC}/bare-metal/${ARCH}"
RODOS_ARCH_SRC2="${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/Include"
RODOS_ARCH_SRC3="${RODOS_SRC}/bare-metal/${ARCH}/hal"
RODOS_ARCH_SRC4="${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/Device/ST/STM32F4xx/Include"
RODOS_ARCH_SRC5="${RODOS_SRC}/bare-metal/${ARCH}/STM32F4xx_StdPeriph_Driver/inc"

SRCS[1]="${RODOS_SRC}/bare-metal-generic"
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}"
SRCS[3]="${RODOS_SRC}/bare-metal/${ARCH}/hal"
SRCS[4]="${RODOS_ARCH_SRC1}"
SRCS[5]="${RODOS_ARCH_SRC1}/startup"
SRCS[7]="${RODOS_ARCH_SRC3}"
SRCS[7]="${RODOS_SRC}/bare-metal/${ARCH}/STM32F4xx_StdPeriph_Driver/src"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH} "  

export INCLUDES_TO_BUILD_LIB=" -I ${RODOS_SRC}/bare-metal-generic  \
    -I ${RODOS_ARCH_SRC1} \
    -I ${RODOS_ARCH_SRC2} \
    -I ${RODOS_ARCH_SRC3} \
    -I ${RODOS_ARCH_SRC4} \
    -I ${RODOS_ARCH_SRC5} "

export CFLAGS_BASICS_COMMON=" -g3 -gdwarf-2 -DHSE_VALUE=${OSC_CLK}"

export CFLAGS_BASICS="${CFLAGS_BASICS_COMMON}  -DSTM32F40_41xxx -DUSE_STM32_DISCOVERY -DUSE_STDPERIPH_DRIVER"
export HWCFLAGS=" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16"
export LINKFLAGS=" -T${RODOS_ARCH_SRC1}/scripts/stm32_flash.ld -nostartfiles -nodefaultlibs -nostdlib -Xlinker --gc-sections -L${RODOS_LIBS}/${TARGET_LIB} -fno-unwind-tables -fno-asynchronous-unwind-tables -lrodos -lm"
export CFLAGS=${CFLAGS}" ${CFLAGS_BASICS} ${HWCFLAGS} "


#export ARM_TOOLS="/opt/arm-tools/bin/"
export ARM_TOOLS=""

export CPP_COMP="${CXX:-${ARM_TOOLS}arm-none-eabi-g++} "
export C_COMP="${CC:-${ARM_TOOLS}arm-none-eabi-gcc} " # only to compile BSP and Drivers from chip provider
export AR="${AR:-${ARM_TOOLS}arm-none-eabi-ar} "

