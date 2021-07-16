
GATEWAY=gateway2-manualtopics.cpp


set -e

rodos-executable.sh  linux-x86  ${GATEWAY}  demo_topics.cpp sender.cpp
mv tst tst-sender
xterm -bg white  -fg black -title senders -e tst-sender &


rodos-executable.sh  linux-x86 ${GATEWAY} demo_topics.cpp receiver1.cpp
mv tst tst-rec1
xterm -geometry 130x23 -bg gray  -fg black -title receiver1 -e tst-rec1 &

rodos-executable.sh linux-x86  ${GATEWAY} demo_topics.cpp receiver2.cpp
mv tst tst-rec2
xterm -bg gray  -fg black -title receiver2 -e tst-rec2 &

echo "CR to close all"
read JA
kill $(jobs -p)

