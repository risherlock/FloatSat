
exteralevents01.cc,

It reacts to external interrupts. In the linux simulation we have only one external
interrupt and it is simulated by the linux command “kill” or “killall”.
Using kill we can generate an signal number 15 (linux stuff).
Use two linux windows. In one compile and execute externalevents.cc.
Execute tst. In the other window call 
	% killall tst



externalevents02.cc
 Obsolte,

externalevents03.cc
to resume a thread from different interrupt sources.



---------------

The interrupt server has to call the corresponding puslishFromInterrupt like in
src/independent/interrupts.cpp
These funcions (interrupt.cpp) have to be linked to the interrupt vector table
of the corresponding CPU

