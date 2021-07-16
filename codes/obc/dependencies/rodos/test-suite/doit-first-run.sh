#!/usr/bin/env bash
#rodos-lib.sh linuxMC

\rm -rf expected-outputs

echo "__________________________________________________________ core fast test programs"
execute-all.sh   linux-x86        core-fast       expected-outputs

echo "__________________________________________________________ core slow test programs"
execute-all.sh   linux-x86        core-slow       expected-outputs

echo "__________________________________________________________ middleware test programs"
execute-all.sh   linux-x86        middleware-tests expected-outputs

echo "__________________________________________________________ middleware gateway"
cd middleware-gateway
./executeit.sh   linux-x86                         expected-outputs
cd ..

echo "__________________________________________________________ Embedded test programs"
execute-all.sh   linux-x86        embedded-test     expected-embedded-outputs


