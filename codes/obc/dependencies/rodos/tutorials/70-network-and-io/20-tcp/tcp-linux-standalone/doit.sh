
cat << EOF
To test:
	You need 2 terminals
	in one start 
	%server
	in the other
	%client

	start first the server
....
EOF


g++ -std=c++11 -o tst-server server.cpp  tcp.cpp
xterm -e tst-server &

g++ -std=c++11 -o tst-client client.cpp  tcp.cpp
tst-client

