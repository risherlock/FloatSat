#include <stddef.h>
#include <stdint.h>

#include "string_pico.h"

namespace RODOS {

size_t strlen(const char *s) {
    size_t len = 0;
    if(!s) return 0;
    while(*s) {
      len++;
      s++;
    }
    return len;
}

size_t strnlen(const char *s, size_t n) {
    size_t len = 0;
    if(!s) return 0;
    while(*s) {
      len++;
      s++;
      if(len >= n) return len;
    }
    return len;
}


char* strcpy(char* destP, const char *s) {
    char* dest = destP;
    if(!s || !dest) return destP;
    while(*s) {
      *dest = *s;
      dest++;
      s++;
    }
    *dest = 0;
    return destP;
}

const char * strstr(const char *s, const char *find)
{
    char c, sc;
    size_t len;

    if ((c = *find++) != 0) {
        len = strlen(find);
        do {
            do {
                if ((sc = *s++) == 0)
                    return nullptr;
            } while (sc != c);
        } while (strncmp(s, find, len) != 0);
        s--;
    }
    return s;
}

const char * strchr ( const char * str, int character ){
    while(*str != 0 && *str != character){
        str++;
    }
    if(*str == character) {
      return str;
    } else {
      return nullptr;
    }
}

void* memset(void* destP, char c, size_t len) {
    if(!destP) return destP ;
    char* dest = static_cast<char*>(destP);
    while(len) {
      *dest = c;
      dest++;  // Added
      len--;
    }
    return destP;
}

void* memcpy(void* destP, const void* sP, size_t len) {
    uint8_t* dest = (uint8_t*)destP;
    const uint8_t* s    = (const uint8_t*)sP;
    if(!dest || !s) return 0;
    while(len) {
	// dest[len] = s[len];
      *dest = *s;
      len--;
      s++;
      dest++;
    }
    return destP;
}
	
int8_t memcmp(const void* a, const void* b, size_t len) {
    const uint8_t* cA = (const uint8_t*)a;
    const uint8_t* cB = (const uint8_t*)b;

    while(len) {
      if (*cA < *cB) return -1;
      if (*cA > *cB) return 1;
      cA++;
      cB++;
      len--;
    }
    return 0;
}



//FILE *stdin=  0;
//FILE *stdout= 0;
//FILE *stderr= 0;

//int __errno;

//void* memcpyXXXX(void* dest, const void* src, int n)
//{ return memmove (dest, src, n); /* principle simplicity: use memmove */ }

char* strncpy(char* destP, const char* src, size_t n) {
  char* dest = destP;
  if(!dest || !src) return 0;
  while (n > 0) {
    *dest = *src;
    if (*src == 0)
      break;
    src++;
    dest++;
    n--; 
  }
  *dest = 0;
  return destP;
}

char* strcpyXXXX(char* dest, const char* src) {
  return (char *)memcpy (dest, src, strlen(src)+1);
}

int8_t strcmp (char const *a, char const *b) {
  if (a == 0)
    return b == 0 ? 0 : -1;

  if (b == 0)
    return +1;

  unsigned char ac, bc;
  do {
    ac= (unsigned char) *a++;
    bc= (unsigned char) *b++;
    if (ac < bc)  return -1;
    if (ac > bc)  return +1;
  }
  while (ac);

  return 0;
}

int8_t strncmp (char const *a, char const *b, size_t n) {
  if (a == 0)
    return b == 0 ? 0 : -1;

  if (b == 0)
    return +1;

  unsigned char ac, bc;
  do {
    ac= (unsigned char) *a++;
    bc= (unsigned char) *b++;
    if (n == 0) return 0;
    n--; // Explizites Inkrement, da dies sonst auch gemacht wird.
    if (ac < bc)  return -1;
    if (ac > bc)  return +1;
  } while (ac);
  // while (ac && bc); 
// Nach den beiden ifs bleibt nur ac == bc, ist ein String kürzer kommen wir
// hier nicht mehr an.

  return 0;
}


}

/**
 * C++ stuff: operator delete.
 * We cannot delete. */
void operator delete (void *) { }

void operator delete (void *ptr, size_t) {
	operator delete (ptr);
}


