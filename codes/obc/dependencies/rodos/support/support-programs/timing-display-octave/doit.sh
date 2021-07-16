
g++ -std=c++11  collision-detector.cpp threadtimings.cpp
mv a.out collsion-detector

g++ -std=c++11 export-to-octave.cpp   threadtimings.cpp

a.out > plotit.m
octave  plotit.m


