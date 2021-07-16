#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# it shows only the templates which have to be implemented for each new port.
# only an example. The hw dependent is empty, will not run!

export TARGET_LIB=template   #used as name for the generated lib
export ARCH=template         #used to select compile directories

SRCS[1]="${RODOS_SRC}/bare-metal-generic" 
SRCS[2]="${RODOS_SRC}/bare-metal/${ARCH}"

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/${ARCH} "  # only for platform-parameter.h 
export INCLUDES_TO_BUILD_LIB=" -I ${RODOS_SRC}/bare-metal-generic "  

export CFLAGS=${CFLAGS}" -m32 "
export LINKFLAGS=" -L ${RODOS_LIBS}/${TARGET_LIB} -lrodos -lm "


#__________________________ Select one: gcc or clang, clang is better to detect warnings but slower
#export CPP_COMP="g++ "
#export C_COMP="gcc "  # only to compile BSP and Drivers from chip provider
export C_COMP="clang "  # only to compile BSP and Drivers from chip provider
export CPP_COMP="clang++ "  



