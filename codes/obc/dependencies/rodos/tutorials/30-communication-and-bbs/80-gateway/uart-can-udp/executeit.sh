\rm -f tst*

set -e

rodos-executable.sh linux commandGenerator.cpp topics.cpp  gateway.cpp 

mv tst tst-sender
xterm -bg white  -fg black -title senders -e tst-sender &

rodos-executable.sh linux topics.cpp  commandReceiver.cpp gateway.cpp 

mv tst tst-receiver
xterm -bg white  -fg black -title receivers -e tst-receiver &



