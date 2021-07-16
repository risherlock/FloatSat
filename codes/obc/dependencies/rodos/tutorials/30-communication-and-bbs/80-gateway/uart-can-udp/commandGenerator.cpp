#include "rodos.h"
#include "topics.h"

/******************************/

static Application senderName("command-generator", 2016);

UserCmd cmdToSend;

class CommandGenerator : public StaticThread<> {
public:
    CommandGenerator() : StaticThread<>("CommandGenerator") { }
    void run () {
        char* s;
        PRINTF("Enter Command:");
        TIME_LOOP(0*SECONDS, 100*MILLISECONDS) {
            if((s = getsNoWait())!= 0 ) { 
                size_t len = strlen(s);
                if (len + 1u > sizeof(cmdToSend.cmdLen)) {
                    len = sizeof(cmdToSend.cmdLen) - 1u;
                }
                cmdToSend.cmdLen = static_cast<int32_t>(len);
                size_t lenToSend = len + 1u + sizeof(cmdToSend.cmdLen);
                strcpy(cmdToSend.cmdLine, s);
                PRINTF("Sending %ld chars: %s\nEnter Command:",
                    static_cast<long>(cmdToSend.cmdLen), s);
                command.publishMsgPart(cmdToSend, lenToSend, true);
            }
        }
    }
} commandGenerator;

