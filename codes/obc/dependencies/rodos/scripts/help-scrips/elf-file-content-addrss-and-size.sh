#!/usr/bin/env bash
#date -d "21 Oct 2015  14:15:16"


if [ $# -ne 1 ]
then
  echo "searchs in .elf file FILE and prints its address and sizes"
  echo "use ADDRESS FILE"
  exit 1
fi

echo
echo "---ADR--|--SIZE--|s|----- NAME----------"
echo "________+________+_+____________________"


#nm --radix=d -S $1 | c++filt  | grep -v \( | sort -n --key=2
nm --radix=d -S $1 | c++filt               | sort -n --key=2

