
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


# warning! It is a loop in file system! Do not forget to remove after using
# tar and git are ok but zip stays in an endless loop until the disc if full
ln -s ../../.. rodos-for-cmake 

\rm -rf build
mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=rodos-for-cmake/cmake/port/${1}.cmake -DEXECUTABLE=ON ..

make


# warning! It is a loop in file system!
# tar and git are ok but zip stays in an endless loop until the disc if full
cd ..
\rm rodos-for-cmake


echo " ____________________________________________"
echo "ready, please see "
echo "build/big-application"

