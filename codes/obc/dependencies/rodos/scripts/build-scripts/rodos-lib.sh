#!/usr/bin/env bash

# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

#______________________________ test environment and parameter

source $(dirname $0)/../quit-if-environment-is-wrong.sh
source $(dirname $0)/../quit-if-arch-is-wrong.sh $1

source $RODOS_VARS_DIR/${1}.sh

export INCLUDES="${INCLUDES} ${INCLUDES_TO_BUILD_LIB} "

#_________________________________ clear the workspace

rm -rf  ${RODOS_TO_LINK}

mkdir   ${RODOS_BUILD}        2> /dev/null
mkdir   ${RODOS_TO_LINK}      2> /dev/null
mkdir   ${RODOS_LIBS}         2> /dev/null
mkdir   ${RODOS_LIBS}/${TARGET_LIB} 2> /dev/null

#___________________________________ compile to .o files

echo "  --------- rodos core ----------------"
source generic-compile-directory.sh ${RODOS_DIR}/src/independent 
source generic-compile-directory.sh ${RODOS_DIR}/src/independent/gateway  

for SRC_X in "${SRCS[@]}"
do
  source generic-compile-directory.sh ${SRC_X}
done


#_________________________________ Support libs
echo "  --------- Support-libs/matlib -------"
source generic-compile-directory.sh ${RODOS_SUPPORT_LIBS}
source generic-compile-directory.sh ${RODOS_SUPPORT_LIBS}/ccsds

#__________________________________ .o files to lib and clear

echo "  --------- create rodoslib.a ---------"

cd ${RODOS_TO_LINK}
rm -f ${RODOS_LIBS}/${TARGET_LIB}/librodos.a
${AR:-ar} rc ${RODOS_LIBS}/${TARGET_LIB}/librodos.a */*.o || exit 1
cd ..

\rm -rf ${RODOS_TO_LINK}   # left my workspace clean


