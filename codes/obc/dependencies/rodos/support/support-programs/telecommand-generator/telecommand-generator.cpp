
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

// Reads from stdin definition of telecommands
// and generates code to iterpret them
//
// As example please read the file telecommands.csv


struct CmdDescr {
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
char      namespaceName[50] = "Common";

#define HEADER \
  "\n\n"\
  "//--- generated file, DO NOT modify\n"\
  "//--- see telecommand-generator.cpp\n"\
  "#pragma once\n\n"

int main() {
    printf(HEADER);
    char inputLine[800];
    char params[800];

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
            strcpy(namespaceName, currCmd.cmdName + 1);
            continue;
        }
        cmds[cmdCnt]  = currCmd;
        cmdCnt++;

        if(cmdCnt >= MAX_CMDS) {
            printf("#warning  to many commands, the rest are lost\n");
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

    printf("namespace %s {\n\n", namespaceName);

    printf("struct TeleCommandCodes {\n");
    for(int cmdIndex = 0; cmdIndex < cmdCnt; cmdIndex++) {
        if(cmds[cmdIndex].subservice == 0 ) {
            printf("\n    static const int %-30s =  %3d;           // service for all %s_* Commands\n",
                cmds[cmdIndex].cmdName, cmds[cmdIndex].service, cmds[cmdIndex].cmdName);
        } else {
            printf("    static const int %-30s = (%3d << 8) | %2d;\n", cmds[cmdIndex].cmdName, cmds[cmdIndex].service, cmds[cmdIndex].subservice);
        }
    }
    printf("};\n");

    //____________________________________________ generate parameter for all telecommands

    /** Eliminate duplicatedd params **/

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
    printf("struct CmdParameter {\n");
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

