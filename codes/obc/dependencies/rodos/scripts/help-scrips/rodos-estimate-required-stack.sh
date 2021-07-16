#!/usr/bin/env bash

source $(dirname $0)/../quit-if-environment-is-wrong.sh


if  [ ! -x "$(command -v g++-10)" ]; then
  echo " Sorry, You need g++-10 , please install first"
  exit 1
fi



echo "  ---------------------------------------- compile lib and generate *.ci"
\rm -f *.ci
rodos-lib.sh linux-stacksize
cat *.ci > ${RODOS_BUILD}/nodes.ci
\rm -f *.ci


echo "  ---------------------------------------- compile your apps and genarate *.ci"
source $RODOS_VARS_DIR/linux-stacksize.sh

${CPP_COMP}   ${CFLAGS} ${CPPFLAGS} ${INCLUDES} -c  ${@}  
\rm -f *.o

cat << EOT
 ------------------------------------- minimum required stack size for all *::run()


 This is the minimum required, pleas add at least 300 bytes for context variables
 on the stack for thread switch and for interrupt service routines which may
 be called when any thread is running 

 The context switch function will deactivate any thread which
 has less than 300 bytes free on Stack.  See:
 src/bare-metal-generic/thread_on_hw.cpp Thread::findNextToRun()

 Warning: This version is not able to consider virtual methods.
 This is specially dangerous in the case of the middleware publish()
 if you do not know the receivers and their virtual put().
 -------------------------------------

EOT
stack-sizes.py ${RODOS_BUILD}/nodes.ci  *.ci | grep run
\rm -f *.ci


