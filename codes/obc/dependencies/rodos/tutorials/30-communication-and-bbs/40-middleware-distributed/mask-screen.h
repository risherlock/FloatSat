

 /**** Generated with support_programs/maskgenerator/generator-splitt.cpp ***/


const char* screen[] = {
"",
"",
"",
"*** See support_programs/maskgen ****",
"",
"",
"Topic-ID  Sender          Receiver",
"--------  --------        --------",
" 11       [      ]        [      ]",
" 12       [      ]        [      ]",
" 13       [      ]        [      ]",
" 14       [      ]        [      ] see demo_topic.cpp: only local",
" 15       [      ]        [      ]",
" 16       [      ]        [      ] [        ] ",
"",
"---------------------------------------------",
"",
"          Number of arrived msgs: [      ]",
"",
"",
 0 };


#define CLEAR_MASK "\x1B[2J\x1B[1;1H"
#define INIT_MASK() PRINTF("%s", CLEAR_MASK); for(int i = 0; screen[i] != 0; i++) PRINTF("%s\n", screen[i]);

