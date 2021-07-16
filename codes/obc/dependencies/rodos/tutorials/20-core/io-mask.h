

/**** Generated with support_programms/maskgen ***/


const char* screen[] = {
    "01. ----- first line ------------------------------------------------------",
    "02.                                                                      --",
    "03. *** This is an example input mask for maskgen.cpp *****              --",
    "04.                                                                      --",
    "05. -----------------------------------------------------------------------",
    "06.",
    "07. counter from Thread aa [      ]         counter from Thread bb [      ] ",
    "09. ",
    "10.                   last input line [                                   ]",
    "11.",
    "12. Please write here your next line: [                                   ]",
    "13.",
    "14. -----------------------------------------------------------------------",
    "15. ----- last line -------------------------------------------------------",
    0
};


#define CLEAR_MASK "\x1B[2J\x1B[1;1H"
#define INIT_MASK()           \
    PRINTF("%s", CLEAR_MASK); \
    for(int i = 0; screen[i] != 0; i++) PRINTF("%s\n", screen[i]);

#define CNTAA "\x1B[7;29H"
#define CNTBB "\x1B[7;69H"
#define READ "\x1B[9;40H"
#define INPUTLINE "\x1B[11;40H"
