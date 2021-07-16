#pragma once

#include "lwip/tcp.h"
#include <stdint.h>

err_t tcp_recv_func(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t);
err_t tcp_accept_func(void* arg, struct tcp_pcb* newpcb, err_t err);
err_t tcp_connected_func(void* arg, struct tcp_pcb* tpcb, err_t err);

namespace RODOS {

class TCPBase {
  public:
    virtual ~TCPBase() { tcp_abort(pcb); }

    virtual int sendData(void* buf, uint16_t len);
    virtual int getData(void* buf, uint16_t maxLen);

  protected:
    pbuf*    recvBuf   = nullptr;
    tcp_pcb* pcb       = nullptr;
    int      errorCode = -1;

  protected:
    uint16_t readPos = 0;

    friend err_t ::tcp_accept_func(void* arg, struct tcp_pcb* newpcb, err_t err);
    friend err_t ::tcp_recv_func(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t);
    friend err_t ::tcp_connected_func(void* arg, struct tcp_pcb* tpcb, err_t err);
};

class HW_TCPServer : public TCPBase {
  public:
    ~HW_TCPServer();

    bool listen(const int32_t portNr);
    bool acceptNewConnection();
    int  sendData(void* buf, uint16_t len) override;
    int  getData(void* buf, uint16_t maxLen) override;
    int  getErrorCode() const;

    tcp_pcb* listenPcb = nullptr;
};

class HW_TCPClient : public TCPBase {
  public:
    ~HW_TCPClient();

    bool reopen(const int32_t portNr, const char* hostname = "localhost");
    int  sendData(void* buf, uint16_t len) override;
    int  getData(void* buf, uint16_t maxLen) override;
    int  getErrorCode() const;
};

} /* namespace RODOS */
