
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
    //char paramType[20][50];
    //char paramName[20][50];
};


static const int MAX_CMDS   = 600;
//static const int MAX_PARAMS = 100;
CmdDescr  cmds[MAX_CMDS];
CmdDescr  currCmd;
//ParmDescr allParams[MAX_PARAMS];
int       cmdCnt  = 0;
//int       paramCnt = 0;

int main() {
    char inputLine[800];
    char params[800];

    //____________________________________________ Read the file
    for (bool done=false; !done; ) {
        char* s = fgets(inputLine, 600, stdin);
        if (s == 0) {
            done = true;
            break;
        }
        params[0]=0;
        memset(&currCmd, 0, sizeof(currCmd));
        sscanf(inputLine,"%s %s %d,%d %[^\n]s",  currCmd.cmdName, currCmd.shortCut,
               &currCmd.service, &currCmd.subservice, params);
        if (currCmd.cmdName[0] ==  0 ) continue; \
        if (currCmd.cmdName[0] == '#') continue; \
        cmds[cmdCnt]  = currCmd;
        cmdCnt++;

        if (cmdCnt >= MAX_CMDS) {
            printf("#warning  too many commands, the rest are lost\n");
            break;
        }
    }

    //____________________________________________ generate links

    printf("mkdir -p shortcuts\n");
    for (int cmdIndex = 0; cmdIndex < cmdCnt; cmdIndex++) {
		if ( !(cmds[cmdIndex].subservice == 0) ) {
                printf("ln -sf generic %s\n", cmds[cmdIndex].cmdName);
				printf("ln -sf ../generic shortcuts/%s\n", cmds[cmdIndex].shortCut);
		}
    }
    return 0;
}

