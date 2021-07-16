
#mask-generator < mask.txt


mkdir -p  ../$2


rodos-executable.sh $1  gateway.cpp demo_topics.cpp    receiver-int64top.cpp
tst  > ../$2/gateway-rec64.txt &
sleep 1
rodos-executable.sh $1  gateway.cpp demo_topics.cpp    receiver-int32top.cpp
tst  > ../$2/gateway-rec32.txt &
sleep 1
rodos-executable.sh $1  gateway.cpp demo_topics.cpp    receiver-doubletop.cpp
tst  > ../$2/gateway-recdoubl.txt &
sleep 1
rodos-executable.sh $1  gateway.cpp demo_topics.cpp    sender.cpp
tst  > ../$2/gateway-sender.txt




