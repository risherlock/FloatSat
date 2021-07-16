#!/usr/bin/env bash

export TARGET_LIB=sf2   #used as name for the generated lib
export ARCH=sf2         #used to select compile directories

SRCS[1]="${RODOS_SRC}/bare-metal-generic"
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}/CMSIS"
SRCS[3]="${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/startup_gcc"
SRCS[4]="${RODOS_SRC}/bare-metal/${ARCH}/drivers"
SRCS[5]="${RODOS_SRC}/bare-metal/${ARCH}/drivers_config/sys_config"
SRCS[6]="${RODOS_SRC}/bare-metal/${ARCH}/ethernet_phy"
SRCS[7]="${RODOS_SRC}/bare-metal/${ARCH}/hal/CortexM3"
SRCS[8]="${RODOS_SRC}/bare-metal/${ARCH}/hal/CortexM3/GNU"
SRCS[9]="${RODOS_SRC}/bare-metal/${ARCH}/rodos"
SRCS[10]="${RODOS_SRC}/bare-metal/${ARCH}/rodos/hal"

# only for platform-parameter.h
export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH}/rodos \
    -I ${RODOS_SRC}/bare-metal-generic/ip/lwip/include"

export INCLUDES_TO_BUILD_LIB=" \
    -I ${RODOS_SRC}/bare-metal-generic \
    -I ${RODOS_SRC}/bare-metal/${ARCH} \
    -I ${RODOS_SRC}/bare-metal/${ARCH}/CMSIS "

# Possible linker scripts:
#export LINKSCRIPT="debug-in-microsemi-smartfusion2-envm.ld"
export LINKSCRIPT="debug-in-microsemi-smartfusion2-envm_w_ddr.ld"
#export LINKSCRIPT="debug-in-microsemi-smartfusion2-esram.ld"
#export LINKSCRIPT="debug-in-microsemi-smartfusion2-external-ram.ld"
#export LINKSCRIPT="production-smartfusion2-execute-in-place.ld"

export DEFINES=" -DMICROSEMI_STDIO_THRU_UART -DMULTICAST "
export CFLAGS_BASICS=" -Wno-long-long -O2 -Wall -fsigned-char -ffunction-sections \
                       -fdata-sections --specs=nano.specs ${DEFINES}"
export HWCFLAGS=" -mcpu=cortex-m3 -mthumb "
export CFLAGS=" ${CFLAGS_BASICS} ${HWCFLAGS} "
export LINKFLAGS=" -T${RODOS_SRC}/bare-metal/${ARCH}/CMSIS/startup_gcc/${LINKSCRIPT} -nostartfiles -nostdlib \
                   -fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections -g3 \
                   -fdata-sections -Xlinker --gc-sections -Wl,-Map,sf2.map \
                   -L${RODOS_LIBS}/${TARGET_LIB} ${APP_LIBS} -lrodos -lm"

#export ARM_TOOLS="/opt/arm-tools/bin/"
if [ -z ${ARM_TOOLS} ]; then
    export ARM_TOOLS=""
fi

export CPP_COMP="${CXX:-${ARM_TOOLS}arm-none-eabi-g++} "
export C_COMP="${CC:-${ARM_TOOLS}arm-none-eabi-gcc} "
export OBJDUMP="${OBJDUMP:-${ARM_TOOLS}arm-none-eabi-objdump} "
export AR="${AR:-${ARM_TOOLS}arm-none-eabi-ar} "
export SIZE="${SIZE:-${ARM_TOOLS}arm-none-eabi-size} "

# check arm-none-eabi version and set flags accordingly
currentver="$($CPP_COMP -dumpversion)"
requiredver="4.9.3"
if [ "$(printf "$requiredver\n$currentver" | sort -V -r | head -n1)" == "$currentver" ] && [ "$currentver" != "$requiredver" ]; then
    # echo "greater than 4.9.3"
    # use the following CPPFLAGS when building with arm-none-eabi v6.x.x
    export CPPFLAGS="-fno-rtti -fno-exceptions -std=c++11 -fno-sized-deallocation "
    # echo "!! gcc version detected is not 4.9.3 - make sure all CPPFLAGS are set correctly !!"
else
    # echo "less or equal than 4.9.3"
    # use the following CPPFLAGS when building with arm-none-eabi v4.9.3
    export CPPFLAGS="-fno-rtti -fno-exceptions -std=c++11 "
fi
