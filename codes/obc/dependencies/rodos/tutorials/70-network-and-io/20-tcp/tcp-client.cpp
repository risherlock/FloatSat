
#include "rodos.h"
#include "hal/tcp.h"

TCPClient tcpClient;

class TcpClientTest : public StaticThread<> {

public:
    char strCnt[90];
    char answer[90];

    void run () {

        PRINTF(" Server shall be started first. I will wait 3 Seconds\n");
        AT(3*SECONDS);

        for(int i = 0; true ; i++) {

            suspendCallerUntil(NOW() + 500*MILLISECONDS);
            if(tcpClient.getErrorCode() != 0) {
                if(!tcpClient.reopen(8877, "localhost") ) {
                    PRINTF("no server running?\n");
                    continue;
                }
            }

            SPRINTF(strCnt, "Client(%d)", i);
            PRINTF("sending %d '%s'\n", (int)strlen(strCnt)+1, strCnt);

            tcpClient.sendData(strCnt, static_cast<uint16_t>(strlen(strCnt)+1));
            int n = tcpClient.getData(answer, 90);
            if(n > 0) {
                PRINTF("received: %d '%s'\n", n, answer);
            } else  { PRINTF("no data\n"); }

        }
    }
} tcpClientTest;

