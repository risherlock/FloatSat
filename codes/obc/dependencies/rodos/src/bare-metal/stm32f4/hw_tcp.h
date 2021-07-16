#pragma once

#include <stddef.h>

// DUMMY!

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


class HW_TCPServer {
public:
  ~HW_TCPServer();

  bool listen([[gnu::unused]] const long portNr) { return false; };
  bool acceptNewConnection() { return false; };
  int sendData([[gnu::unused]] void* buf, [[gnu::unused]] size_t len) { return 0; };
  int getData([[gnu::unused]] void* buf, [[gnu::unused]] size_t maxLen) { return 0; };
  int getErrorCode() const { return -1; };
};

class HW_TCPClient {
public:
  ~HW_TCPClient();

  bool reopen([[gnu::unused]] const long portNr, [[gnu::unused]] const char* hostname = "localhost") { return false; };
  int sendData([[gnu::unused]] void* buf, [[gnu::unused]] size_t len) { return 0; };
  int getData([[gnu::unused]] void* buf, [[gnu::unused]] size_t maxLen) { return 0; };
  int getErrorCode() const { return -1; };
};

#ifndef NO_RODOS_NAMESPACE
} // namespace
#endif
