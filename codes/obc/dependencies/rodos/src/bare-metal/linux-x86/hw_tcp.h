#pragma once

#include "hal/tcp.h"
#include <sys/types.h>

namespace RODOS {

class HW_TCPServer {
  /**
   * TCP Uses 2 types of sockets, the connection socket (cleint_sock)
   * and the listen socket (listen_sock).
   * The Goal is to separate the connection phase from the datai
   *  exchange phase.
   **/

  public:
    ~HW_TCPServer();

    bool listen(const uint16_t portNr);
    bool acceptNewConnection();
    ssize_t  sendData(void* buf, size_t len);
    ssize_t  getData(void* buf, size_t maxLen);
    int  getErrorCode();

  protected:
    const int MAX_CLIENTS = 10;
    int       listen_sock = -1;
    int       client_sock = -1;
    int       errorCode   = -1;
};

class HW_TCPClient {
  public:
    ~HW_TCPClient();

    bool reopen(const uint16_t portNr, const char* hostname = "localhost");
    ssize_t  sendData(void* buf, size_t len);
    ssize_t  getData(void* buf, size_t maxLen);
    int  getErrorCode();

  protected:
    int sock      = -1;
    int errorCode = -1;
};

} // namespace RODOS
