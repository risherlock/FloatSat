
set -e

cd client/
  mask-generator-splitt < mask.txt
  rodos-executable.sh linux-x86  *.cpp ../gateway.cpp  ../topics.cpp 
cd ..

cd server/
  rodos-executable.sh linux-x86  *.cpp ../gateway.cpp  ../topics.cpp 
cd ..

xterm -fg green -bg black -title "TTP Receiver" -e server/tst &
sleep 2 # it has to be, else no sync possible

xterm -fg blue -bg white -title "TTP sender" -e client/tst &
xterm -fg blue -bg white -title "TTP sender" -e client/tst &
xterm -fg blue -bg white -title "TTP sender" -e client/tst &
xterm -fg blue -bg white -title "TTP sender" -e client/tst &


echo "press return to kill all created jobs"
read JA

echo $(jobs -p)
kill $(jobs -p)

