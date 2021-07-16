
set -e

while true
do 
    echo "1pps" at $(date +%H:%M:%S)
    killall tst 
    sleep 1
done
