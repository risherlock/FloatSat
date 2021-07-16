#include "rodos.h"
#include "support-libs.h"


Tokenizer tok;
char* inputCmd;

char cmdName[16];

#define MAX_PARAMS 8

static class CmdDec : public StaticThread<> {
public:
    void run() {
        PRINTF("Enter command and parameter\n");
        while(1) {
            suspendCallerUntil(NOW() + 200*MILLISECONDS);
            inputCmd = getsNoWait();
            if(!inputCmd) continue;

            //___________________________________ analyse input string 
            tok.init(inputCmd);
            char* token = tok.next();
            if(token == 0) continue;

            strncpy(cmdName, token, 7);

            //___________________________________ Scan its parameter
            PRINTF("cmd : '%s' ", cmdName);
            int paramCnt; 
            for(paramCnt = 0; (token = tok.next()) != 0; paramCnt++) {
                int64_t param = s2int(token);
                PRINTF(" %lld ", static_cast<long long>(param));
            }
            PRINTF("   --- Total %d params\n", paramCnt);
            PRINTF("Enter command and parameter\n");
        }
    }

} cmdDec;



