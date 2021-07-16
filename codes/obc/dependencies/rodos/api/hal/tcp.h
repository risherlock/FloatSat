
#pragma once

#include <stdint.h>
#include "hw_tcp.h"

/**
 * @file tcp.h
 * @date 2018/06/03
 * @author Sergio Montenegro
 *
 * TCP interface
 */

namespace RODOS {

class TCPServer {
protected:
  HW_TCPServer tcp;

public:
  bool listen(const uint16_t portNr);
  bool acceptNewConnection();
  int32_t sendData(void* buf, uint16_t len);   // not just send, due to name collision with lwip
  int32_t getData(void* buf, uint16_t maxLen); // not just get due to name collision
  int32_t getErrorCode();
};

class TCPClient {
protected:
  HW_TCPClient tcp;

public:
  bool reopen(const uint16_t portNr, const char* hostname = "localhost");
  int32_t sendData(void* buf, uint16_t len);   // not just send due to name collision with lwip
  int32_t getData(void* buf, uint16_t maxLen); // not just get due to name collision
  int32_t getErrorCode();
};

}  // namespace
