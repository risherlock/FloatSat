#!/usr/bin/env bash

if [ $# -ne 1 ]
then
  cat << EOT
please say which platform:
    linux-makecontext
    linux-x86
    posix
    raspberrypi3
    stm32f4
    sf2
see
    cmake/port/*

and...
    do not forget 
        source sentenvs.sh
    in the rodos root directory

EOT
  exit 1
fi

source $(dirname $0)/../quit-if-environment-is-wrong.sh


cd $RODOS_DIR 
\rm -rf build
mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=cmake/port/${1}.cmake ..


make -j rodos


# Compile Tutorials
#-----------------

cmake -DCMAKE_TOOLCHAIN_FILE=cmake/port/${1}.cmake -DEXECUTABLE=ON ..

make -j helloworld
make -j helloworld-multiple
make -j basic
make -j time
make -j priority
make -j combuffer
make -j fifo
make -j fifo-sync
make -j semaphore
make -j semaphore-macro
make -j semaphore-deadlock
make -j event
make -j receiver-simple
make -j receiver-commbuf
make -j receiver-putter
make -j receiver-sync




