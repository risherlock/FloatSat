
/**
 * @file tcp.cpp
 * @date 2018/06/03
 * @author Sergio Montenegro
 *
 *
 * TCP interface, generic implementation
 *
 */

#include "tcp.h"

// following includes depend on the implementation: Posix or else lwip. 
// The rest stays the same

//__________________________________ Platform dependent
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//__________________________________



namespace RODOS {

//_______________________________________________________________________________

TCPServer::TCPServer(const int32_t portNr) {
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

TCPServer::~TCPServer() {
    close(listen_sock);
}

bool TCPServer::acceptNewConnection() {
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

int TCPServer::sendData(void* buf, int len)    { return ::send(client_sock, buf, len, 0);  }
int TCPServer::getData (void* buf, int maxLen) { return recv(client_sock, buf, maxLen, 0); }

//_______________________________________________________________________________

TCPClient::TCPClient(const long portNr, const char* hostname) {

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

TCPClient::~TCPClient() { close(sock); }

int TCPClient::sendData(void* buf, int len)    { return ::send(sock, buf, len, 0);  }
int TCPClient::getData (void* buf, int maxLen) { return recv(sock, buf, maxLen, 0); }

} // namepsace
