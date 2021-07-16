#pragma once

#include <stddef.h>
#include <stdint.h>

namespace RODOS {

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t n);
char* strcpy(char* dest, const char *s);
char* strncpy(char* dest, const char* src, size_t n);
void* memcpy(void* dest, const void* src, size_t len);
void* memset(void* dest, char c, size_t len);
int8_t memcmp(const void* a, const void* b, size_t len);
int8_t strcmp (char const *a, char const *b) ;
int8_t strncmp (char const *a, char const *b, size_t n);
char* strcpyXXXX(char* dest, const char* src);
extern const char * strstr(const char *s, const char *find);
extern const char * strchr (const char * str, int character);

// bcopy und bzero sind laut POSIX standard nicht mehr erwünscht, wir sollten sie besser ganz weglassen, das ist einfacher.
// Die Argumente source und destinationen sind bei bcopy vertauscht gegenüber
// memcpy, das führt nur zu Irrtümern. Also auch daher sollten wir es vermeiden
inline void*bcopy(const void* src, void* dest, size_t len) {
    return memcpy(dest, src, len);
}
inline void* bzero(void* dest, size_t len) {
    return memset(dest, '\0', len);
}




}  // namespace

