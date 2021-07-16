#!/bin/sh

GATEWAY=gateway-forward-all.cpp



rodos-executable.sh linux-x86 ${GATEWAY} demo_topics.cpp receiver.cpp
mv tst tst-recv

