#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# runs on Posix using 64 bit mode

export TARGET_LIB=on-posix64   #used as name for the generated lib
export ARCH=posix64         #used to select compile directories

SRCS[1]="${RODOS_SRC}/on-posix"
SRCS[2]="${RODOS_SRC}/on-posix/hal"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/on-posix "  # only for platform-parameter.h 
export INCLUDES_TO_BUILD_LIB=" "

export LINKFLAGS=" -L ${RODOS_LIBS}/${TARGET_LIB} -lrodos -lm -lrt -lpthread "

#__________________________ Select one: gcc or clang, clang is better to detect warnings but slower
#export CPP_COMP="g++ "
#export C_COMP="gcc "  # only to compile BSP and Drivers from chip provider
export C_COMP="${CC:-clang} "  # only to compile BSP and Drivers from chip provider
export CPP_COMP="${CXX:-clang++} "  


#POSIX-warning: 
#To execute your compiled code, please use no more than one core. 
#Please call
#    "sudo taskset -c 0 tst"


