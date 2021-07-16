#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

export TARGET_LIB=skith  #used as name for the generated lib

export INCLUDES=${INCLUDES}" -I ${RODOS_SRC}/bare-metal/stm32f4/platform-parameter/skith "  # only for platform-parameter.h
export OSC_CLK=16000000     # SKITH

source $RODOS_VARS_DIR/stm32f4.sh


