
# Waning: For Linux we do not need the ipstack.cpp, but
# for embedded you have to compile and link to client and server


rodos-executable.sh linux-x86 tcp-server.cpp
xterm -e tst &

rodos-executable.sh linux-x86 tcp-client.cpp
tst
