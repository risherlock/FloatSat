
#pragma once

/**
 * @file tcp.h
 * @date 2018/06/03
 * @author Sergio Montenegro
 *
 * TCP interface
 */

namespace RODOS {

class TCPServer {
    const int MAX_CLIENTS = 10;
    int listen_sock;
    int client_sock;
public:
    int errorCode = -1;
    TCPServer(const int portNr);
    ~TCPServer();

    bool acceptNewConnection();
    int  sendData(void* buf, int len);
    int  getData (void* buf, int maxLen);
};

class TCPClient {
    int sock;
public:
    int errorCode = -1;
    TCPClient(const long portNr, const char* hostname = "localhost");
    ~TCPClient();

    int sendData(void* buf, int len);
    int getData (void* buf, int maxLen);
};

} // namepsace

