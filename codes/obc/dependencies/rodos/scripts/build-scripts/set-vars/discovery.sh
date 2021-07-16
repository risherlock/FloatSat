#! /bin/bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

export TARGET_LIB=discovery  # used as name for the generated lib

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/stm32f4/platform-parameter/discovery "  # only for platform-parameter.h
export OSC_CLK=8000000     # STM32F4Discovery, STM32F429Discovery

source $RODOS_VARS_DIR/stm32f4.sh


