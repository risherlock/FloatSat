#include "stdio.h"
#include "stdlib.h"

/*** Seee mask.txt and example.cpp ****/

struct Field {
   char name[100];
   int x, y;
};

int main() {
    char inputLine[500];
    Field field[300];
    int fieldCnt = 0;

    FILE* fileMakScreen = fopen("mask-screen.h", "w");
    FILE* fileMak      = fopen("mask.h", "w");
    
    fprintf(fileMakScreen, "\n\n /**** Generated with support_programs/maskgenerator/generator-splitt.cpp ***/\n");
    fprintf(fileMak,       "\n\n /**** Generated with support_programs/maskgenerator/generator-splitt.cpp ***/\n");
    fprintf(fileMakScreen, "\n\nconst char* screen[] = {\n");

    int ycnt=0;
    while(fgets(inputLine, 500, stdin)) {
        ycnt++;
        bool inklammer = false;
	int charcnt =0;
        for (int i=0; inputLine[i] != 0; i++) {
            if(inputLine[i] == ']')  { inklammer = false; field[fieldCnt].name[charcnt] = 0; fieldCnt++; charcnt =0;}
            if(inklammer)            { field[fieldCnt].name[charcnt] = inputLine[i]; charcnt++; inputLine[i] = ' ';}
            if(inputLine[i] == '[')  { inklammer = true; field[fieldCnt].x = i+1;field[fieldCnt].y = ycnt;}
            if(inputLine[i] == '\n' ){ inputLine[i] = 0;}
        }
        fprintf(fileMakScreen, "\"%s\",\n", inputLine);
    }
    fprintf(fileMakScreen, " 0 };\n\n\n");

    fprintf(fileMakScreen, "#define CLEAR_MASK \"\\x1B[2J\\x1B[1;1H\"\n");
    fprintf(fileMakScreen, "#define INIT_MASK() PRINTF(\"%%s\", CLEAR_MASK); for(int i = 0; screen[i] != 0; i++) PRINTF(\"%%s\\n\", screen[i]);\n\n");

    for(int i = 0; i < fieldCnt; i++) {
        fprintf(fileMak, "#define %s  \"\\x1B[%d;%dH\"\n", field[i].name, field[i].y, field[i].x+1);
        //printf("%s (%d, %d)\n", field[i].name, field[i].x, field[i].y); 
    }
 
    fclose(fileMakScreen);
    fclose(fileMak);
    return 0;
}
