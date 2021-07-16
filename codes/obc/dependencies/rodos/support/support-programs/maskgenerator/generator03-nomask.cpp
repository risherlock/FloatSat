#include "stdio.h"
#include "stdlib.h"

/*** Seee mask.txt and example.cpp ****/

struct Field {
   char name[100];
   int x, y;
};

int main() {
    unsigned char inputLine[500];
    Field field[300];
    int fieldCnt = 0;
    
    fprintf(stdout, "\n\n /**** Generated with support_programs/maskgenerator/generator-nomask.cpp ***/\n");
    fprintf(stdout, "\n\nconst char* screen[] = {0,0,0,0};\n");

    int ycnt=0;
    while(fgets((char*)inputLine, 500, stdin)) {
        ycnt++;
        bool inklammer = false;
	int charcnt =0;
        int xcnt = 0;
        for (int i=0; inputLine[i] != 0; i++) {
            xcnt++;
            if(inputLine[i] == 0xe2) xcnt -= 2; // multichar line character eg ┌────┐
            if(inputLine[i] == ']')  { inklammer = false; field[fieldCnt].name[charcnt] = 0; fieldCnt++; charcnt =0;}
            if(inklammer)            { field[fieldCnt].name[charcnt] = inputLine[i]; charcnt++; inputLine[i] = ' ';}
            if(inputLine[i] == '[')  { inklammer = true; field[fieldCnt].x = xcnt;field[fieldCnt].y = ycnt;}
            if(inputLine[i] == '\n' ){ inputLine[i] = 0;}
        }
    }

    fprintf(stdout, "#define CLEAR_MASK \" \"\n");
    fprintf(stdout, "#define INIT_MASK()\n");

    for(int i = 0; i < fieldCnt; i++) {
        fprintf(stdout, "#define %s  \"\\n%s = \"\n", field[i].name, field[i].name);
    }
    return 0;
}
