
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "tcp.h"
/**
 * TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * */

RODOS::TCPServer  tcpServer (8877);

int main([[gnu::unused]] int argc, [[gnu::unused]] char *argv[]) {

    while (true) {
        // open a new socket to transmit data per connection
        while(!tcpServer.acceptNewConnection() ) {
            printf("waiting for a new client\n");
            usleep(500000);
        }

        char  buffer[90];
        int cnt = 0;
        int nodataCnt = 0;

        while(1) {
            int len = tcpServer.getData(buffer, 90);
            if(len > 0) {
                printf("received: %d '%s'\n", len, buffer);
                sprintf(buffer, "Server(%d)", cnt++);
                if(cnt % 2) tcpServer.sendData(buffer, strlen(buffer)+1);
                nodataCnt = 0;
            }
            if(len == 0) {
                printf("waiting for data\n");
                nodataCnt++;
                if(nodataCnt > 10) {
                    tcpServer.acceptNewConnection();
                    printf("waiting for new connection\n");
                }
                usleep(500000);
            }
            if(len < 0) {
                printf("connection cloded\n");
                break;
            }
        } // all data from a client
    } // loop
    return 0;
}

