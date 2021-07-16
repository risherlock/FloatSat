

echo "   __________________________________________________________"
date
rodos-clear.sh
echo "templates for new ports"
rodos-lib.sh template

echo "linux-x86-lib"
rodos-lib.sh linux-x86
echo "linux-makecontext-lib"
rodos-lib.sh linux-makecontext
echo "on-posix-lib"
rodos-lib.sh on-posix
echo "on-posix64-lib"
rodos-lib.sh on-posix64
echo "discovery-lib"
rodos-lib.sh discovery
echo "skith-lib"
rodos-lib.sh skith
echo "sf2-lib"
rodos-lib.sh sf2
echo "raspberrypi3"
rodos-lib.sh raspberrypi3
echo "   __________________________________________________________"

