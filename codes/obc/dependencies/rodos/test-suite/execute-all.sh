#!/usr/bin/env bash

#rodos-lib.sh linuxMC

if [ $# -ne 3 ]
then
    echo "Usage: $(basename $0) <architecture_name> <sources-dir> <output-dir>"
  exit 1
fi

set -e

mkdir -p  $3

TEST_LEN=-DTIME_TO_END=40

for i in $2/*.cpp
do
    FILENAME_NO_DIR=${i##*/}      # rm all chars until last '/'
    BASE=${FILENAME_NO_DIR%.*}    # rm from the end until the first '.'
    NOW=$(date +%H:%M:%S)

    printf "$NOW: %50s -> $3/$BASE.txt\n"  $i

    \rm -f tst
    rodos-executable.sh $1 $TEST_LEN  -I. $i
    {
        tst   > $3/$BASE.txt 
    } &> /dev/null &

    # no more than 4 jobs simultaneusliy (I assume 4 cores)
    NUM=$(jobs | wc -l)
    if [ "$NUM" -gt 3 ] ; then
        echo "          waiting for jobs to conclude"
        wait
    fi

done

echo "          __________________________________"
jobs
wait
rm -f tst
echo "          __________________________________"
jobs


