// The class Thread is extended by a custom run() procedure, which 
// writes Hello World to the UART with PRINTF.

#include "rodos.h"

class HelloWorld : public StaticThread<>
{
	void run()
	{
		PRINTF("Hello World!\n");
	}
} hello_world;