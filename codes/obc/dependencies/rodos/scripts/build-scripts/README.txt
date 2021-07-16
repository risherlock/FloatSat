
# Author Sergio Montenegro (uni Wuerzburg), Sept 2019

This directory contains bash-sh scripts to compile and link applications on RODOS.
For small applications and examples you may call these scripts directly.
For bigger applications which usually consist of many software components
distributed in many files and directories and eventually written by different
programmers we recommend to write link and compile scripts which call
this basic scripts.

... Or else
create your own 
   make, cmake, meson, maven, gradle, scons, ninja, premake, 
   MakeMe, Buck, Ant, rake, cake, Qmake, Ebuild, autotool, etc, etc
script and just use the rodos lib ${RODOS_LIBS}/${ARCH}/librodos.a
as ready to use lib and for the includes just copy the configuration
from the corresponding xxx-set-vars.sh

------
Some tutorials have an own execute and 
compile script which is based on these basic scripts.
    See and read in each directory if there is a
        doit.sh
        executeit.sh
    they shall be simple to understand
----

IMPORTANT:

First you shall call 
	source setenvs.sh
from the rodos root directory.

This will set path variable and other environment variables.
Then you may call any other script from ANYWHERE. 
They will use the absolute paths defined in setenvs.sh

Here there are some generic scripts:
    generic-*.sh  (do not use directly)
    rodos-*.sh
They need some variables which shall be set in
    set-vars/*
e.g.
    set-vars/xxx.sh

here xxx is the name of the target architecture
for example linux-x86, linux-makeContext
on-posix, on-posix64, stm32f4, sf2, ppc, sparc, leon etc etc.

The scripts rodos-*.sh requires as first parameter the name
of the target architecture. e.g.
    rodos-lib.sh discovery
please note, a file named xxx-set-vars.sh, in this example
stem32f4-set-vars.sh has to exist.


To compile and build software systems these scripts write
to ${RODOS_BUILD} which is defined in setenvs.sh
The final executable will be in the same directory where
you call rodos-executable.sh xxx <list of source files>

Scrips you will use:
    rodos-lib.sh <architecture>     
        compile the required rodos sources and generates the rodoslib.a
        it will be places in ${RODOS_LIBS}/<architecture>/rodoslib.a
        (subdirectory of ${RODOS_BUILD})

    rodos-executable <architecture> <list of source files>
        compiles the <list of source files> and links it using rodoslib.a

These scripts will call (use)
     generic-compile-directory.sh <directory to compile>
this scripts expects all variables set in xxx-set-vars.sh
and compiles all sources found in <directory to compile>
all generated object files will be places in
    ${RODOS_TO_LINK}/o<absolute path of directory, using "-" instead of "/">
    (subdirectory of ${RODOS_BUILD})

-------
To see how to use, go to the tutorials and read the executeit.sh files
begin with tutorials/first-steps

------
Example:

first set the environment variables: 
Directories and default compiler options

    source setenvs.sh 

Generate the RODOS lib for some target architectures

    rodos-lib.sh linux-x86
    rodos-lib.sh linux-makecontext
    rodos-lib.sh on-posix


Test some tutorial examples using different
target architectures

    cdrodos 
    cd tutorials/
    cd 20-core/

    rodos-executable.sh linux-x86 semaphore-worm.cpp 
    tst

    rodos-executable.sh on-posix semaphore-worm.cpp 
    tst

    rodos-executable.sh discovery semaphore-worm.cpp 
    .. load the elf file tst on your development board and boot it



