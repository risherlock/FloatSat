

/**** Generated with support_programs/maskgenerator/generator-simple.cpp ***/


const char* screen[] = {
"",
"",
"",
"",
"",
"Time sync (SNTP) - Server",
"----------------------------------------------------------",
"",
"Last client node:  [         ]",
"Client requests:   [         ]",
"",
"----------------------------------------------------------",
"Current time: [            ] sec.microsecs",
"",
 0 };


#define CLEAR_MASK "\x1B[2J\x1B[1;1H"
#define INIT_MASK() PRINTF("%s", CLEAR_MASK); for(int i = 0; screen[i] != 0; i++) PRINTF("%s\n", screen[i]);
