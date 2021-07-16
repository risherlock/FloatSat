#!/usr/bin/env bash
#rodos-lib.sh linuxMC


\rm -rf tmp

echo "__________________________________________________________ core fast test programs"
execute-all.sh   linux-x86        core-fast       tmp

echo "__________________________________________________________ matlib-test"
execute-all.sh   linux-x86        matlib-test     tmp

echo "__________________________________________________________ core slow test programs"
execute-all.sh   linux-x86        core-slow       tmp

echo "__________________________________________________________ middleware test programs"
execute-all.sh   linux-x86        middleware-tests tmp

echo "__________________________________________________________ middleware gateway"
cd middleware-gateway
./executeit.sh   linux-x86                         tmp
cd ..

echo "__________________________________________________________ Special cases"
cd special-cases
./executeit.sh   linux-x86                         tmp
cd ..


echo "__________________________________________________________ Embedded test programs"
execute-all.sh   linux-x86        embedded-test     tmp-embedded



cat << EOT
__________________________________________________________ Diffs
 ++++ preemptiontest-and-ceiler and combuffer-test are not deterministric 
      compare by hand
 please compare (diff -rq or meld) :
 diff -rq   expected-outputs           tmp
 diff -rq   expected-embedded-outputs  tmp-embedded
_________________________________________________________________
EOT
meld    expected-outputs           tmp
meld    expected-embedded-outputs  tmp-embedded
