

 /**** Generated with support_programs/maskgenerator/generator-splitt.cpp ***/


const char* screen[] = {
"",
"",
"",
"",
"",
"Time sync (SNTP) - Client",
"----------------------------------------------------------",
"",
"Current request: [         ]",
"",
"Server                        Client",
"----------------------------  ----------------------------",
"SENT: [                ] ms.  SENT: [                ] ms.",
"RCV:  [                ] ms.  RCV:  [                ] ms.",
"----------------------------------------------------------",
"Offset                        Delay",
"----------------------------  ----------------------------",
"[                ] ms.        [                ] ms.",
"----------------------------------------------------------",
"",
"Interrupt time: [                ] milliseconds",
"",
"Send Time:      [          ]",
 0 };


#define CLEAR_MASK "\x1B[2J\x1B[1;1H"
#define INIT_MASK() PRINTF("%s", CLEAR_MASK); for(int i = 0; screen[i] != 0; i++) PRINTF("%s\n", screen[i]);

