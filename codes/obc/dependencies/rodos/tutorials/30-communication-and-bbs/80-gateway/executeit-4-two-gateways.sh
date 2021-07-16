
echo "the sender has two gateways, one sended only topic counter1 and the other counter2"
echo "the receiver nodes have both receiver1 and 2, and subscribe both tocpics, but"
echo "they have only one gateway, which gets only one topic"


set -e

rodos-executable.sh  linux-x86   gateway-manualtopic-1.cpp gateway-manualtopic-2.cpp   demo_topics.cpp sender.cpp
mv tst tst-sender
xterm -bg white  -fg black -title senders -e tst-sender &


rodos-executable.sh linux-x86  gateway-manualtopic-1.cpp demo_topics.cpp receiver1.cpp receiver2.cpp
mv tst tst-rec1
xterm -geometry 130x23 -bg gray  -fg black -title receiver1 -e tst-rec1 &


rodos-executable.sh linux-x86  gateway-manualtopic-2.cpp demo_topics.cpp receiver1.cpp receiver2.cpp
mv tst tst-rec2
xterm -bg gray  -fg black -title receiver2 -e tst-rec2 &


echo "CR to close all"
read JA
kill $(jobs -p)

