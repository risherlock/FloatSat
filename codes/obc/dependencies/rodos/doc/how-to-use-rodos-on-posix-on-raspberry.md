
How to use rodos on posix on raspberry running the Raspbian OS
==============================================================

After successful installation and configuration of Raspbian:

1. Install git.

	$ sudo apt-get install git

2. RODOS depends on the Clang compiler front-end tool, which is not included in Raspbian by default and has to be manually installed.

	$ sudo apt-get install clang

3. Download latest RODOS version from Gitlab.

	$ git clone https://gitlab.com/rodos/rodos

Go to your RODOS directory.

4. Create the necessary environment variables. This has to be repeated every time a new session or terminal is started.

	$ source setenvs.sh

5. Generate the RODOS library for the target architecture, in this case POSIX.

	$ rodos-lib.sh posix

At this point, RODOS is fully configured and ready for use. The tutorial section includes programs meant do demonstrate the functionality of RODOS. Each chapter has a README file that explains how to run its programs, but in general the procedure is as follows:

	$ rodos-executable.sh posix YOU_PROGAM_LIST.cpp
	$ tst

Some tutorials, e.g. the distributed ones are some how more complex to compile and execute. In these
cases use the script `execute*`  which you will find in the corresponding directory.


**NOTES:**

- For the sake of the tutorials, it is recommended to use RODOS with a graphical desktop. Install a Raspbian version that includes a graphical desktop or manually install one.

- Some tutorials require the xterm terminal emulator, which has to be installed as well:

	$ sudo apt-get install xterm
