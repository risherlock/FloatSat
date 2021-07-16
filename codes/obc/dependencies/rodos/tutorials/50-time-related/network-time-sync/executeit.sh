
set -e

cd server
rodos-executable.sh linux-x86 ../gateway.cpp clearscreen.cpp ../topics.cpp ts-server.cpp
xterm -bg white -fg black -title "TimeSync Server" -e tst &
cd ..
echo "server running, wait 3 seconds to start each client"
sleep 3

cd client
rodos-executable.sh linux-x86 ../gateway.cpp clearscreen.cpp ../topics.cpp ts-client.cpp
xterm -bg white -fg black -title "TimeSync Client [1]" -e tst &

sleep 3
xterm -bg white -fg black -title "TimeSync Client [2]" -e tst &

sleep 3
xterm -bg white -fg black -title "TimeSync Client [3]" -e tst &


sleep 3 # the can not be so fast after the last exterm
echo "press return to kill all created jobs"
sleep 3 # the can not be so fast after the last exterm
read JA

echo $(jobs -p)
kill $(jobs -p)


