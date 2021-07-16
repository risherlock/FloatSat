#include "hal/tcp.h"

namespace RODOS {

bool TCPServer::listen(const uint16_t portNr)     { return tcp.listen(portNr); }
bool TCPServer::acceptNewConnection()         { return tcp.acceptNewConnection(); }
int32_t TCPServer::sendData(void *buf, uint16_t len)   { return static_cast<int32_t>(tcp.sendData(buf, len)); }
int32_t TCPServer::getData(void *buf, uint16_t maxLen) { return static_cast<int32_t>(tcp.getData(buf, maxLen)); }
int32_t TCPServer::getErrorCode()                 { return tcp.getErrorCode(); }

bool TCPClient::reopen(const uint16_t portNr, const char *hostname) { return tcp.reopen(portNr, hostname); }
int32_t TCPClient::sendData(void *buf, uint16_t len)                     { return static_cast<int32_t>(tcp.sendData(buf, len)); }
int32_t TCPClient::getData(void *buf, uint16_t maxLen)                   { return static_cast<int32_t>(tcp.getData(buf, maxLen)); }
int32_t TCPClient::getErrorCode()                                   { return tcp.getErrorCode(); }

}
