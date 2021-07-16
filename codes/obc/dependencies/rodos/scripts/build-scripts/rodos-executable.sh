#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

# the first parameter $1 is the architecture name, 
# all other parameter  ${@:2} are files to be compiled

#______________________________ test environment and parameter

source $(dirname $0)/../quit-if-environment-is-wrong.sh
source $(dirname $0)/../quit-if-arch-is-wrong.sh $1
 
source $RODOS_VARS_DIR/${1}.sh

if [ !  -e ${RODOS_LIBS}/$1/librodos.a ]
then
  echo -e "\n\n****************** Ohhhh ***********"
  echo "RODOS was not compiled for $1! I will do it now"
  rodos-lib.sh $1
fi


#cppcheck      ${@:2} 
${CPP_COMP}   ${CFLAGS} ${CPPFLAGS} ${INCLUDES} -o tst ${@:2}  ${LINKFLAGS} 


