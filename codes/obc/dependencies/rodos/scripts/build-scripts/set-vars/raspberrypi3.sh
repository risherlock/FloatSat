#!/usr/bin/env bash

# Author Michael Zehrer (Uni Wuerzburg), October 2019

# to create a boot SD card, see doc/how-to-make-a-boot-SD-card-for-Raspberry.md

export TARGET_LIB=raspberrypi3   #used as name for the generated lib
export ARCH=raspberrypi3         #used to select compile directories

SRCS[1]="${RODOS_SRC}/bare-metal-generic"
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}"
SRCS[3]="${RODOS_SRC}/bare-metal/${ARCH}/hal"
SRCS[4]="${RODOS_SRC}/bare-metal/${ARCH}/startup"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH}/include " # only for platform-parameter.h

export INCLUDES_TO_BUILD_LIB=" \
    -I ${RODOS_SRC}/bare-metal-generic \
    -I ${RODOS_SRC}/bare-metal/${ARCH}/include"


export CFLAGS_BASICS="-O2 -Wall -g0"
export HWCFLAGS="\
    -mfloat-abi=soft \
    -mcpu=cortex-a53 \
    -nostartfiles"
#    -mfpu=neon-fp-armv8

export CFLAGS="${CFLAGS_BASICS} ${HWCFLAGS}"
export LINKFLAGS="-Wl,-T${RODOS_SRC}/bare-metal/${ARCH}/scripts/linkerscript.ld \
    -Wl,--entry=_exception_table -Wl,--gc-sections \
    -nodefaultlibs -nostdlib \
    -ffreestanding -fno-strict-aliasing -ffunction-sections -fdata-sections\
    -fno-unwind-tables -fno-asynchronous-unwind-tables \
    -L${RODOS_LIBS}/${TARGET_LIB} \
    -lrodos -lm -lgcc"

# Check if the U-Boot tool 'mkimage' is available (package 'u-boot-tools')
if command -v mkimage >/dev/null 2>&1; then
  export UBOOT_MKIMAGE=mkimage
fi

# set PATH to whereever your ARM compiler is located; usually you should
# have symlinks in /usr/bin so this would work out-of-the box
export ARM_TOOLS=""

export CPP_COMP="${CXX:-${ARM_TOOLS}arm-none-eabi-g++} "
export C_COMP="${CC:-${ARM_TOOLS}arm-none-eabi-gcc} "
export AR="${AR:-${ARM_TOOLS}arm-none-eabi-ar} "
export OBJCOPY="${OBJCOPY:-${ARM_TOOLS}arm-none-eabi-objcopy} "
export OBJDUMP="${OBJDUMP:-${ARM_TOOLS}arm-none-eabi-objdump} "
