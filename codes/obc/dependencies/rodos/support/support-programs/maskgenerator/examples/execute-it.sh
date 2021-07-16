echo "CR start all examples, each one needs ca 6 seconds"; read JA

echo "CR to execute generator01-simple" ; read JA
  ../mask-generator-simple < mask.txt > mask.h
  g++ generator01-simple.cpp 
  a.out
  \rm a.out

echo 
echo
echo "CR to execute the same but with nice lines" ; read JA
  ../mask-generator-simple < mask-lines.txt > mask.h 
  g++ generator01-simple.cpp 
  a.out
  \rm a.out

echo 
echo
echo "CR to execute generator03-nomask" ; read JA
  ../mask-generator-nomask < mask.txt > mask.h
  g++ generator01-simple.cpp  # be aware: the same program linke the first example
  a.out
  \rm a.out

echo 
echo
echo "CR to execute generator04-print-by-name" ; read JA
  ../mask-generator-print-by-name < mask.txt > mask.h
  g++ generator04-print-by-name.cpp
  a.out
  \rm a.out

echo 
echo
echo "CR to execute generator05-print-all-fields" ; read JA
  ../mask-generator-print-all-fields < mask.txt  > mask.h
  g++ generator05-print-all-fields.cpp
  a.out
  \rm a.out

echo 
echo
\rm -f a.out mask.h mask-fields.h






