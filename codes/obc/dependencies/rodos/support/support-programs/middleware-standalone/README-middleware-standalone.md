Get started
===========

This is an implementation of the RODOS middleware without RODOS.
This allows you to communicate (send and receive) with RODOS applications 
(applications running or a RODOS node) from other
nodes which do not run on RODOS.

To test, first open 2 shells, one for RODOS and one for Linux.
they shall communicate using the RODOS topics.

As with all RODOS programmes, you have to run the **setenvs.sh** script in the RODOS root directory. The is ONLY done in the Rodos shell.

```
    source setenvs.sh
```

In the Linux shell, go to example/on-linux, in the RODOS shell to example/on-rodos

```
    cd example/on-linux
    cd example/on-rodos
```

On Linux
========

Run **doit.sh**.

```
    doit.sh
```

Make sure, that the script is executable. It will create a file called **a.out** which is the executable. Just start it.

```
    doit.sh
    a.out
```

On RODOS
========

On RODOS, compile **send_receive_on_rodos.cpp** and run the file **tst**.

```
    rodos-executable.sh linuxMC send_receive_on_rodos.cpp
    tst
```
If you have run **rodos-lib.sh** with **linux** instead of **linuxMC**, you have to use it with **linux**.


Now, you should have 2 terminals running programmes which send data to each other.

