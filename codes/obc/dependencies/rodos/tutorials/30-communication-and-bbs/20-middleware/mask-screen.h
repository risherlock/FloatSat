

 /**** Generated with support_programs/maskgenerator/generator-splitt.cpp ***/


const char* screen[] = {
"*** See support_programs/maskgen to generate mask.h****",
"",
"publishing counter1 [           ]  counter2  [           ]",
"publishing counter2 [           ] (shall be allways 100)",
"publishing postion  [       ][       ][       ]",
"_____________________________________________________________________",
"",
"AsyncFifo  position: [       ][       ][       ] (waiting 20 seconds)",
"Combuf     position: [       ][       ][       ] (waiting 20 seconds)",
"SnyFifo    counter1  [       ]",
"",
"Putter MultipleReceiver [   ] bytes, data: [       ] Topic: [       ]",
"",
"Trhread    counter1  ",
"           put: len [     ] topic [      ]       Data [       ]",
"           run:                                       [       ]",
"",
"_____________________________________________________________________",
"receiver simple:",
"Subscriber         (Example1)  [   ] bytes, data: [       ] From [     ][     ]",
"SubscriberReceiver (Example2)                     [       ]",
"SubscriberFunction (Example3)                     [       ]",
"",
 0 };


#define CLEAR_MASK "\x1B[2J\x1B[1;1H"
#define INIT_MASK() PRINTF("%s", CLEAR_MASK); for(int i = 0; screen[i] != 0; i++) PRINTF("%s\n", screen[i]);

