
#include "scanf-substitue.h"

bool Tokenizer::isSeparator(char c) {
     // if(c == 0) return true;
     for(int i = 0; separators[i] != 0; i++) {
         if(c == separators[i]) return true;
     }
     return false;
}

char* Tokenizer::next() {
    if(!isValid()) return 0;
    // find begin of token
    while(isSeparator(*ptr)) ptr++;

    if(*ptr == 0) return 0;
    char* retVal = ptr;
    // mark end of token
    while(*ptr != 0 && !isSeparator(*ptr)) ptr++;
    if(*ptr != 0) {
        *ptr = 0;
        ptr++;
    }
    return retVal;
}



//converts s to int, s MUST start with '-' or '0' to '9'. Terminates by a non digit.
int64_t s2int(char *s) {
    int64_t retVal = 0;
    bool negativ = false;

    if(*s == '-') {
        negativ = true;
        s++;
    }

    while(*s >= '0' && *s <= '9') {
        retVal = retVal*10 + (*s - '0');
        s++;
    }
    if(negativ) retVal = -retVal;
    return retVal;
}


//_______________________________________________________________-

char* strCpyUntilChar(char* dest, char* source, char terminator) {
    if(source == 0) return 0;
    dest[0] = 0;
    for(int i = 0; source[i] ; i++) {
        if(source[i] == terminator || source[i] == '\n') return source + i + 1;
        dest[i] = source[i];
        dest[i+1] = 0;
    }
    return 0;
}


