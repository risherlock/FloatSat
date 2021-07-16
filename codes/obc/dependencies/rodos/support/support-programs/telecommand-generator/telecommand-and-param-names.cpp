
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

// Reads from stdin definition of telecommands
// and generates code to iterpret them
//
// As example please read the file telecommands.csv


struct CmdDescr {
    char appName[150];
    char cmdName[150];
    char shortCut[150];
    int  service;
    int  subservice;
    char paramType[20][50];
    char paramName[20][50];
    char paramUnit[20][50];
};

struct ParmDescr {
    bool duplicated;
    char paramType[50];
    char paramName[50];
    int  posInBuf;
};


static const int MAX_CMDS   = 600;
static const int MAX_PARAMS = 100;
CmdDescr  cmds[MAX_CMDS];
CmdDescr  currCmd;
ParmDescr allParams[MAX_PARAMS];
int       cmdCnt  = 0;
int       paramCnt = 0;

#define HEADER \
  "\n\n"\
  "//--- generated file, DO NOT modify\n"\
  "//--- see telecommand-and-param-names.cpp\n"\
  "#pragma once\n"\
  "namespace Common {\n"\
  "\n\n"

int main() {
    printf(HEADER);
    char inputLine[800];
    char params[800];
    char appName[150];

    //____________________________________________ Read the file
    for(bool done=false; !done; ) {
        char* s = fgets(inputLine, 600, stdin);
        if(s == 0) {
            done = true;
            break;
        }
        for(s = inputLine; *s; s++) if(*s == '#') { *s = 0; break; } // remove comments

        params[0]=0;
        memset(&currCmd, 0, sizeof(currCmd));
        sscanf(inputLine,"%s %s %d,%d %[^\n]s",  currCmd.cmdName, currCmd.shortCut,
               &currCmd.service, &currCmd.subservice, params);
        if(currCmd.cmdName[0] ==  0 ) continue; 
        if(currCmd.cmdName[0] == '@') {
            if ( strcmp(appName, (currCmd.cmdName)+1) ) {
                strncpy(appName, (currCmd.cmdName)+1, strlen(currCmd.cmdName) );
            }
            continue;
        }
        strcpy(currCmd.appName, appName);
        cmds[cmdCnt]  = currCmd;
        cmdCnt++;

        if(cmdCnt >= MAX_CMDS) {
            printf("#warning  too many commands, the rest are lost\n");
            break;
        }
        int pos = 0;
        /** Read all params **/
        for(int i = 0, fields = 4; fields == 4; i++) {
            currCmd.paramType[i][0] = 0;
            currCmd.paramName[i][0] = 0;
            currCmd.paramUnit[i][0] = 0;
            if(params[0] == 0)   break;
            if(params[0] == ' ') break;
            if(params[0] == '#') break;
            fields = sscanf(params,"%s %s %s %[^\n]s", currCmd.paramType[i], currCmd.paramName[i], currCmd.paramUnit[i], params);
            cmds[cmdCnt-1]  = currCmd;
            strcpy(allParams[paramCnt].paramType, currCmd.paramType[i]);
            strcpy(allParams[paramCnt].paramName, currCmd.paramName[i]);
            allParams[paramCnt].duplicated = false;
            allParams[paramCnt].posInBuf = pos;

            //count position
            if (!strcmp(currCmd.paramType[i], "int8_t" ))  pos+=1;
            if (!strcmp(currCmd.paramType[i], "int16_t"))  pos+=2;
            if (!strcmp(currCmd.paramType[i], "int32_t"))  pos+=4;
            if (!strcmp(currCmd.paramType[i], "int64_t"))  pos+=4;
            if (!strcmp(currCmd.paramType[i], "uint8_t" )) pos+=1;
            if (!strcmp(currCmd.paramType[i], "uint16_t")) pos+=2;
            if (!strcmp(currCmd.paramType[i], "uint32_t")) pos+=4;
            if (!strcmp(currCmd.paramType[i], "uint64_t")) pos+=4;
            if (!strcmp(currCmd.paramType[i], "char*"  ))  pos+=1;
            paramCnt++;
        }

    }

    //____________________________________________ generate telecommand codes

    printf("struct TelecommandCodes {\n");
    printf("    const char* app; const char* cmdName; const char* cmdMnemonic; int service; int subservice; const char* paramDef[20];\n");
    printf("};\n");
    printf("TelecommandCodes telecommands[] = {\n");
    for (int cmdIndex = 0; cmdIndex < cmdCnt; cmdIndex++) {
		if ( !(cmds[cmdIndex].subservice == 0) ) {
			if ( !(strcmp(cmds[cmdIndex].paramType[0], "")) ) { // No parameters
				printf("    { \"%s\", \"%s\", \"%s\", %d, %d, { 0, 0 } },\n",
				    	    cmds[cmdIndex].appName, cmds[cmdIndex].cmdName, cmds[cmdIndex].shortCut, cmds[cmdIndex].service, cmds[cmdIndex].subservice);
			}
			else {
				printf("    { \"%s\", \"%s\", \"%s\", %d, %d, { ",
					cmds[cmdIndex].appName, cmds[cmdIndex].cmdName, cmds[cmdIndex].shortCut, cmds[cmdIndex].service, cmds[cmdIndex].subservice);
				int paramIndex = 0;
				do {			
					printf("\"%s\", \"%s\", ", cmds[cmdIndex].paramType[paramIndex], cmds[cmdIndex].paramName[paramIndex]);
				} while ( (strcmp(cmds[cmdIndex].paramType[++paramIndex], "")) );
		
				printf("0, 0 } },\n");
			}
		}
    }
	printf("    { 0, 0, 0, 0, 0 }\n");
    printf("};\n");

    //____________________________________________ generate parameter for all telecommands

    /** Eliminate duplicated params **/

    for(int i = 0; i < paramCnt; i++) {
        for(int j = 0; j < i; j++) {
            if((strcmp(allParams[i].paramName, allParams[j].paramName) == 0) &&
                    (strcmp(allParams[i].paramType, allParams[j].paramType) == 0) ) allParams[i].duplicated = true;
        }
    }

    /** parameter list independent from commands **/

    printf("\n");
    printf("#ifndef CMD_PARAMETER_STRUCT\n");
    printf("#define CMD_PARAMETER_STRUCT\n");
    printf("\nstruct CmdParameter {\n");
    for(int i = 0; i < paramCnt; i++) {
        if(!allParams[i].duplicated) printf("    %-10s %s; \n", allParams[i].paramType, allParams[i].paramName);
    }

    printf("\n    inline void deserialize(char* buf) {\n");
    for(int i = 0; i < paramCnt; i++) {
        if(!allParams[i].duplicated) {
            if(strcmp(allParams[i].paramType, "char*") != 0) {
                strcat(allParams[i].paramName, ",");
                printf("        BasicSerializers::deserialize(%-10s buf+%2d);\n", allParams[i].paramName, allParams[i].posInBuf);
            } else {
                printf("        %s = buf+%2d;\n", allParams[i].paramName, allParams[i].posInBuf);
            }
        }
    }

    printf("    }\n");
    printf("};\n");
    printf("#endif\n");
    printf("} // namespace\n");

    return 0;
}

