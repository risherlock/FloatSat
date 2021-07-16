

Warning:
UDP Broadcast (our gateway) works only if you have
an internet connection or  a local linux-gateway.
If you want to test it in a computer without internet thenn
do this:
   $ sudo ifconfig eth0 192.168.1.234 netmask 255.255.255.0 up
   $ sudo route add default gw 192.168.1.234 eth0
---
To reuse internet:
    $ sudo ifconfig eth0 down
    $ sudo dhcpcd eth0 

if you do not have dhcpcd then try dhclient


--------------------


first read the sender.cpp and receiver*.cpp

we distribute this 3 tasks in 3 nodes and use different configurations
of gateways to interconnect them.

now go througth execute-1-* .. execute-n-*
and study the corresponding gateyaw* configuration used on each



middleware-distributed anpassen


