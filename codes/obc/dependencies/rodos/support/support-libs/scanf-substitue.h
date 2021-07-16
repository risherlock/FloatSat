
#pragma once
#include "stdint.h"

//  substitute for scanf , simple scanf functions or tokenizer 

// breaking a stream of text up into words (tokens) separated by blanc or comma (by default), 
// or by self defined separators.
struct Tokenizer {
    char* ptr;          //MAY NOT point to a constant char* as it will be modified by next()
    const char* separators;

    Tokenizer() { separators = " ,;\n";}
    bool isSeparator(char c);
    void init(char* s) { ptr = s; };
    void setSeparators(const char *separatorsP) { separators = separatorsP; }
    bool isValid() { return ptr != 0 && *ptr != 0; }
    char* next(); // pointer to first char of next token, will  be terminated by 0
    void  getAll(char* token[], int maxTokens) { for(int i = 0; i < maxTokens; i++) token[i] = next(); }
};

//number can start with '-'terminates by the first non-digit character (not '0' to '9'), eg. Tokens from Tokenizer
#define atoi s2int
extern int64_t s2int(char *s);

extern char* strCpyUntilChar(char* dest, char* source, char terminator);

