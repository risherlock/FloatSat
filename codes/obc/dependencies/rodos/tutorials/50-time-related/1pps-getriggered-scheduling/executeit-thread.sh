
set -e

rodos-executable.sh  linux-x86 scheduler-1pps-getriggert.cpp  example-threads-1pps.cpp

cat << EOT
______________________________________________
       to test please
       in one term start tst
       in other term type killall tst
       or execute the script generate-1pps.sh
________________________________________________


EOT

tst
