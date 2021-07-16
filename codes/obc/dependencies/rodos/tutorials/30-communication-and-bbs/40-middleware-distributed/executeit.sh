
#mask-generator < mask.txt

if [ $# -ne 1 ]
then
    echo "Usage: $(basename $0) <architecture_name>"
  exit 1
fi

set -e

rodos-executable.sh $1  gateway.cpp clearscreen.cpp demo_topics.cpp sender1*
mv tst tst-sender
xterm -bg white  -fg black -title senders -e tst-sender &


rodos-executable.sh $1 gateway.cpp clearscreen.cpp  demo_topics.cpp receiver*
mv tst tst-receiver-all
xterm -bg white  -fg black -title receivers -e tst-receiver-all &

rodos-executable.sh $1 gateway.cpp clearscreen.cpp  demo_topics.cpp receiver11.cpp
mv tst tst-receiver-11
xterm -bg white  -fg black -title receivers11 -e tst-receiver-11 &

rodos-executable.sh $1 gateway.cpp clearscreen.cpp  demo_topics.cpp receiver15.cpp
mv tst tst-receiver-15
xterm -bg white  -fg black -title receivers15 -e tst-receiver-15 &

echo "CR to close all"
read JA

kill $(jobs -p)

