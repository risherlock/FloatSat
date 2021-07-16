

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#include "tcp.h"

RODOS::TCPClient tcpClient(8877, "localhost");

int main() {

    char strCnt[90];
    char answer[90];

    for(int i = 0; i < 20; i++) {
        usleep(500000);
        sprintf(strCnt, "Client(%d)", i);
        printf("sending %d '%s'\n", (int)strlen(strCnt)+1, strCnt);

        tcpClient.sendData(strCnt, strlen(strCnt)+1);
        int n = tcpClient.getData(answer, 90);
        if(n > 0) {
            printf("received: %d '%s'\n", n, answer);
        } else  { printf("no data\n"); }
    }
    return 0;
}
