
Example of a big application with many sub apps




first go to your rodos dir and call
	% srouce setenvs

Then...
from this directory call
	%source setenvs


see all scripts in
	make/*


first compile RODOS for your target, eg
        %linux-lib
or.....
        %posix-lib
        %ppc405-lib
        %stm32f4-lib
etc.....


to compile and link all applications listed in
   make/list-of-bb-to-compile
just call
        %doit-mission-linux <dir-name>
example
	 %doit-mission-linux workingdir-dev
or...
	the doit-linux.sh in each workingdir does the same
	without requesting the dir parameter

The execute the tst file in the directory where you call the doit*

see first 
	workingdir-dev0
then
	workingdir-dev1
and so on....


you can pass as parameter more bb to compile and link
else it will compile and link only the bb listed in
	list-of-bb-to-compile + common + contracs

to compile just a building block, go to its direcktore
        %cd my_bb_example
        %linux-compile-bb       (or ppc405-compile-bb, etc etc)


