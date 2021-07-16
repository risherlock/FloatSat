
/**
 * @file tcp.cpp
 * @date 2018/06/03
 * @author Sergio Montenegro
 *
 *
 * TCP interface, generic implementation
 *
 */

#include "hal/tcp.h"

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

HW_TCPServer::~HW_TCPServer() {
    close(listen_sock);
}

bool HW_TCPServer::listen(const uint16_t portNr) {
    sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(portNr);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    errorCode                      = 0;

    if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { errorCode = -2; }
    if((bind(listen_sock, (sockaddr*)&server_address, sizeof(server_address))) < 0) { errorCode = -3; }
    if(::listen(listen_sock, MAX_CLIENTS) < 0) { errorCode = -4; }

    /** Set to nonblocking: no wait if no data is ready */
    fcntl(listen_sock, F_SETFL, O_NONBLOCK);

    return true;
}

bool HW_TCPServer::acceptNewConnection() {
    sockaddr_in client_address;
    socklen_t   client_address_len = 0;
    if(client_sock >= 0) { close(client_sock); }
    client_sock = accept(listen_sock, (sockaddr*)&client_address, &client_address_len);
    if(client_sock < 0) {
        errorCode = -5;
        return false;
    }
    //xprintf("client connected with ip address: '%s'\n", inet_ntoa(client_address.sin_addr));

    /** Set to nonblocking: no wait if no data is ready */
    //fcntl(client_sock, F_SETFL, O_NONBLOCK); Warning: Does not work!

    return true;
}

ssize_t HW_TCPServer::sendData(void* buf, size_t len)   { return ::send(client_sock, buf, len, 0); }
ssize_t HW_TCPServer::getData(void* buf, size_t maxLen) { return recv(client_sock, buf, maxLen, 0); }
int HW_TCPServer::getErrorCode()                 { return errorCode; }

//_______________________________________________________________________________

HW_TCPClient::~HW_TCPClient() { close(sock); }

bool HW_TCPClient::reopen(const uint16_t portNr, const char* hostname) {
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, hostname, &server_address.sin_addr);
    server_address.sin_port = htons(portNr);

    errorCode = 0;
    if(sock >= 0) { close(sock); }
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { errorCode = -2; }
    if(connect(sock, (sockaddr*)&server_address, sizeof(server_address)) < 0) { errorCode = -3; }

    /** Set to nonblocking: no wait if no data is ready */
    fcntl(sock, F_SETFL, O_NONBLOCK);

    return errorCode == 0;
}

ssize_t HW_TCPClient::sendData(void* buf, size_t len)   { return ::send(sock, buf, len, 0); }
ssize_t HW_TCPClient::getData(void* buf, size_t maxLen) { return recv(sock, buf, maxLen, 0); }
int HW_TCPClient::getErrorCode() { return errorCode; }

} // namespace RODOS
