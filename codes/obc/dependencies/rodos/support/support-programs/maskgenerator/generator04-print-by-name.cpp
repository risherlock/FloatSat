#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*** Seee mask.txt and example.cpp ****/

struct Field {
   char name[100];
   int x, y;
};

int main() {
    unsigned char inputLine[500];
    Field field[300];
    int fieldCnt = 0;

    fprintf(stdout, "\n\n /**** Generated with support_programs/maskgenerator/generator-print-by-name.cpp ***/\n");
    fprintf(stdout, "\n\nconst char* screen[] = {\n");

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
        fprintf(stdout, "\"%s\",\n", inputLine);
    }
    fprintf(stdout, " 0 };\n\n\n");

    fprintf(stdout, "#define CLEAR_MASK \"\\x1B[2J\\x1B[1;1H\"\n");
    fprintf(stdout, "#define INIT_MASK() fputs(CLEAR_MASK, stdout); for(int i = 0; screen[i] != 0; i++) puts(screen[i]);\n\n");

    puts("struct FieldPos {");
    puts("   const char* fieldname;");
    puts("   const char* gotoPos;");
    puts("};");
    putchar('\n');
    puts("FieldPos fieldPos[] = {");

    for(int i = 0; i < fieldCnt; i++) {
        char name[100];
        strcpy(name, field[i].name);
        for(int j = 0; name[j]; j++) if(name[j] == ' ') name[j] = 0;
        fprintf(stdout, "    { \"%s\", \"\\x1B[%d;%dH%%%ds\" },\n", name, field[i].y, field[i].x+1, (int)strlen(field[i].name));
    }
    puts("    {0, 0 }");
    puts("};");
    putchar('\n');

    fprintf(stdout, "void printMaskField(const char* field, const char* val) {\n");
    fprintf(stdout, "    for(int i= 0; fieldPos[i].fieldname != 0; i++) {\n");
    fprintf(stdout, "        if(strcmp(fieldPos[i].fieldname, field) != 0)  continue;\n");
    fprintf(stdout, "        printf(fieldPos[i].gotoPos, val);\n");
    fprintf(stdout, "        return;\n");
    fprintf(stdout, "    }\n");
    fprintf(stdout, "}\n\n");

    return 0;
}

