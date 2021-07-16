#!/usr/bin/env bash

source $(dirname $0)/../quit-if-environment-is-wrong.sh

MYDIR=$PWD
cd $RODOS_DIR

#__________________________________________
echo -n "   occupied disc: "
du -sh .
# save size before deletions
SIZE_B=$(du -sk . | cut -f 1 | head -n 1)

#__________________________________________

CLEAR_LIST=$(find . -type f | grep -E "\.o$|\.out$|sf2.map$|tst$|tst-|tst_|\.elf$|\.bin$|\.hex$|\.img$|\.ci$|temp$|a.out")

rm -f  ${CLEAR_LIST}
rm -rf ${RODOS_BUILD}

rm -rf ${RODOS_ROOT}/test-suite/tmp*

#__________________________________________
echo -n "   after clear occupied disc: "
du -sh .
# save size after deletions
SIZE_A=$(du -sk . | cut -f 1 | head -n 1)
SIZE_D=$((${SIZE_B} - ${SIZE_A}))
echo "   ${SIZE_D}K  = $((${SIZE_D} / 1024)) M byted deleted"

#__________________________________________
cd $MYDIR

