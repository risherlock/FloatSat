
mkdir -p  ../$2

rodos-executable.sh $1  gets-async.cpp
(echo "string1"; sleep 2; echo "string2"; sleep 2; echo "string3") | tst > ../$2/gets-async.txt

sleep 1
rodos-executable.sh $1  getsnowait.cpp
(echo "string1"; sleep 2; echo "string2"; sleep 2; echo "string3") | tst > ../$2/getsnowait.txt


