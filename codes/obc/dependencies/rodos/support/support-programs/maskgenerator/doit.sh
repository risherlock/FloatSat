
\rm -f mask-generator*

g++ -o mask-generator-simple                 generator01-simple.cpp 
ln -s mask-generator-simple                  mask-generator

g++ -o mask-generator-splitt                 generator02-splitt.cpp 
g++ -o mask-generator-nomask                 generator03-nomask.cpp 
g++ -o mask-generator-print-by-name          generator04-print-by-name.cpp
g++ -o mask-generator-print-all-fields       generator05-print-all-fields.cpp

