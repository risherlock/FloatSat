
cat << END_OF_MESSAGE

  _______________________________________________________________
Sorry but this time the doit is "doit your self"


see first ../lowpassfilter/doit and test there



all example programs generate a matlab  oder octave program.
to see the result you need to execute octave on the output

to see the original data (big noise) call
   octave measurements.m

Then try the simple calmanfilter-for-informatic, ony scalar:
for example
    linux-executable nadirfinder-scalar-test.cpp
    tst > temp.m
WARNING: you have to stop it using ^C (control C)
See the results using
    octave temp.m


The more comprex with attitude determination:
    linux-executable  nadirfinder.cpp  filteredsensors.cpp 
    tst > temp.m
WARNING: you have to stop it using ^C (control C)
See the results using
    octave temp.m



END_OF_MESSAGE

