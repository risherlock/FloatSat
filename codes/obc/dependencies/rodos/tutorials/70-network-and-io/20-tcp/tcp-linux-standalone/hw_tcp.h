
#pragma once

//#include "hw_udp.h"


#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>


/**
 * @file tcp.h
 * @date 2018/06/03
 * @author Sergio Montenegro
 *
 *
 * TCP interface
 *
 */

namespace RODOS {

class TCPServerHW {
    const int MAX_CLIENTS = 10;
    int listen_sock;
    int client_sock;
    int errorCode = -1;

public:
    TCPServerHW(const int32_t portNr) {
        sockaddr_in  server_address;

        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family      = AF_INET;
        server_address.sin_port        = htons(portNr);
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        errorCode = 0;

        if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { errorCode = -2; }
        if ((bind(listen_sock, (sockaddr *)&server_address, sizeof(server_address))) < 0) { errorCode = -3; }
        if (listen(listen_sock, MAX_CLIENTS) < 0)                  { errorCode = -4; }

        /** Set to nonblocking: no wait if no data is ready */
        fcntl(listen_sock, F_SETFL, O_NONBLOCK);
    }

    ~TCPServerHW() {
        close(listen_sock);
    }

    bool acceptNewConnection() {
        sockaddr_in  client_address;
        socklen_t    client_address_len = 0;
        client_sock = accept(listen_sock, (sockaddr *)&client_address, &client_address_len);
        if(client_sock < 0 ) { errorCode = -5; }
        if(client_sock < 0) return false;
        printf("client connected with ip address: '%s'\n", inet_ntoa(client_address.sin_addr));

        /** Set to nonblocking: no wait if no data is ready */
        //fcntl(client_sock, F_SETFL, O_NONBLOCK);

        return true;
    }

    int sendData(void* buf, int len)   { return ::send(client_sock, buf, len, 0);  }
    int getData(void* buf, int maxLen) { return recv(client_sock, buf, maxLen, 0); }
    bool readyToGet()              { }
};

class TCPClientHW {
    int sock;
    int errorCode = -1;

public:
    TCPClientHW(const long portNr, const char* hostname = "localhost") {

        sockaddr_in   server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        inet_pton(AF_INET, hostname, &server_address.sin_addr);
        server_address.sin_port = htons(portNr);

        errorCode = 0;
        if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { errorCode = -2; }
        if (connect(sock, (sockaddr*)&server_address, sizeof(server_address)) < 0) { errorCode = -3; }

        /** Set to nonblocking: no wait if no data is ready */
        fcntl(sock, F_SETFL, O_NONBLOCK);
    }

    ~TCPClientHW() { close(sock); }

    int sendData(void* buf, int len)   { return ::send(sock, buf, len, 0);  }
    int getData(void* buf, int maxLen) { return recv(sock, buf, maxLen, 0); }

};

} // namepsace

