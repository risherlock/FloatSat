Get RODOS
=========

Inf you have questions, please contact sergio.montenegro@uni-wuerzburg.de

(By the way, you can format this file using the program okular)

There are two easy ways to get RODOS:

1.   download RODOS [ here ]( https://gitlab.com/rodos/rodos/-/archive/master/rodos-master.tar.bz2 ) and extract it with:
```
	tar -jxvf rodos-master.tar.gz
```

2.  clone  it from gitlab with

```
    git clone https://gitlab.com/rodos/rodos.git
    cd rodos
```


Take a look to the short introduction in  

```
	https://en.wikipedia.org/wiki/Rodos_%28operating_system%29 
	https://de.wikipedia.org/wiki/Rodos_%28Betriebssystem%29
```

How to use
==========

All these instructions are only for **Linux**  
and please use the **bash** shell!

Each directory has one or more `*README*` files.   
For each directory you visit, please read first these files,  
and study the "doc" directory.

1  Tools
--------

If you do not have the clang/c++ tools, please install first:

```
    sudo apt-get install gcc-multilib g++-multilib
    sudo apt-get install clang 
for ARM:
    sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi
```

2  Sent shell variables
-----------------------
Every time you start a new session or a new terminal
set the environment variables directories and default compiler options by calling

```
    cd <your rodos rood directory>
    source setenvs.sh 
```

3  RODOS libs
-------------
Generate the RODOS lib for some target architectures, for example

```
        rodos-lib.sh linux-x86
        rodos-lib.sh linux-makecontext
        rodos-lib.sh on-posix
        rodos-lib.sh discovery   # you will need the ARM Corsscompiler
    or generate all libraries
        rodos-all-libs.sh
```
Please not, for some you will need the crosscompiler tools

4 first test
------------ 

(optional) Test some tutorial examples using different
target architectures

```
    cdrodos     #"cdrodos" is defined as alias in setenvs.sh
    cd tutorials
    cd 20-core

    rodos-executable.sh  linux-x86 semaphore-worm.cpp 
    tst       # terminate with control-c

    rodos-executable.sh  on-posix semaphore-worm.cpp 
    tst

    rodos-executable.sh  linux-makecontext  semaphore-worm.cpp 
    tst

```

5 Begin learning 
----------------

To begin, learning rodos

```
    cdrodos 
    cd tutorials/10-first-steps
```
1. read the `*README*` files  
2. read the `execute-example*.sh` scripts
3. read the corresponding source code
4. execute the `execute-example*.sh` scripts
5. see the results.


6 other tutorials
-----------------

Go to all other tutorials and to the same.


7 Port to other Hardware
------------------------

To port to other platforms, see  
    rodos/src/bare-metal/template
see all `*REAME*` files  all pdf and all examples  
see infos in   
    src/bare-metal/template/README-how-to-port


Need help?
----------

If you need help, please contact me.

Cheers
Sergio Montenegro
sergio.montenegro@uni-wuerzburg.de
