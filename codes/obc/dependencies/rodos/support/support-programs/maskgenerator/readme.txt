

Generates macros (#define) for terminals VT 100 (eg. xtern, gnom-erm, etc)
using space sequences
See
   http://www.termsys.demon.co.uk/vtansi.htm

In the mask you define fields and the generator creates macros 
To move the cursor to the corresponding position so you can print
the values to the masks

all generators are very similar. They have the same front end
but generate different macros for different applications

all generators read vom standard input and write to standard output
Some generate an extra file with extra help for your program


The best, you see the examples in directory
	examples


