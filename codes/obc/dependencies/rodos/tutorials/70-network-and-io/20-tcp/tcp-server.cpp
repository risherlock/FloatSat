
#include "rodos.h"
#include "hal/tcp.h"

RODOS::TCPServer tcpServer;

class TcpServerTest : public StaticThread<> {

    void run() {

    	tcpServer.listen(8877);

        while (true) {

            while(!tcpServer.acceptNewConnection() ) {
                PRINTF("waiting for a new client\n");
                suspendCallerUntil(NOW() + 500*MILLISECONDS);
            }

            char  buffer[90];
            int cnt = 0;
            int nodataCnt = 0;

            while(1) {
                int len = tcpServer.getData(buffer, 90); // Warning: it seems non watind do not work for the first msg!
                if(len > 0) {
                    PRINTF("received: %d '%s'\n", len, buffer);
                    SPRINTF(buffer, "Server(%d)", cnt++);
                    if(cnt % 2) tcpServer.sendData(buffer, static_cast<uint16_t>(strlen(buffer)+1));
                    nodataCnt = 0;
                }
                if(len == 0) {
                    PRINTF("waiting for data\n");
                    nodataCnt++;
                    if(nodataCnt > 10) {
                        tcpServer.acceptNewConnection();
                        PRINTF("waiting for new connection\n");
                    }
                    suspendCallerUntil(NOW() + 500*MILLISECONDS);
                }
                if(len < 0) {
                    PRINTF("connection closed\n");
                    break;
                }
            } // all data from a client
        } // loop
    }

} tcpServerTest;

