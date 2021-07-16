#pragma once

#include <stdint.h>

namespace RODOS {

bool isspace(const char c);
bool isdigit(const char c);
bool isalpha(const char c);
bool isupper(const char c);

int32_t strToInt32_t (const char * nptr, const char ** endptr, uint8_t base);

}  // namespace


