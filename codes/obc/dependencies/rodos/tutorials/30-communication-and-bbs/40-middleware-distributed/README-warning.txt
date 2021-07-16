
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



