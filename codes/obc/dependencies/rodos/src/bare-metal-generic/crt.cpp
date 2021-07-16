/**
 * @author Andre Bartke <dev@bartke.cc>
 *
 * @brief C run-time to set up the environment.
 *
 * Linkerscript considerations:
 *   List of con-/destructors contains:
 *    - number of entries
 *    - entries
 *    - zero
 *   Note: 64-bit pointers are 8-byte in size so we have to use QUADs
 *    /8 function pointer is 8-bytes in size
 *    -2 minus number of entries, minus zero
 *
 * The linkerscript shall include the following in the .rodata/.ctors section:
 *   __CTOR_LIST__ = .;
 *   LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
 *   *(.ctors)
 *   LONG(0)
 *   __CTOR_END__ = .;
 *
 * The linkerscript shall surround the .bss section with these symbols:
 *   __bss_start = .;
 *   .bss : {
 *     ...
 *   }
 *   __bss_end = .;
 */

#ifdef __USE_GLOBAL_CRT__

#include <string_pico.h>
#include <stdint.h>

// prototype for the main() function
extern int main(int argc, char** argv);

// these symbols are introduced by the linker script
extern void (*__CTOR_LIST__)();
extern void (*__CTOR_END__)();
extern uint8_t __bss_start;
extern uint8_t __bss_end;

// This symbol is called from assembly, we need to use extern "C" to disable
// C++ name mangling and ensure C linkage.
#ifdef __cplusplus
extern "C" {
#endif

void _start() {
  void (**ctor)(void) = &__CTOR_LIST__;
  size_t entries;

  // make sure .bss is clear
  uint8_t *bss_start = &__bss_start;
  uint8_t *bss_end = &__bss_end;
  memset(bss_start, 0, bss_end - bss_start);

  /* The list starts with an integer containing the number of entries,
   * we have to increment the pointer to the first constructor.
   */

  // 32-bit
  if(sizeof(void*) == 4) {
    entries = *(uint32_t *)ctor++;
  }

  // 64-bit
  if(sizeof(void*) == 8) {
    entries = *(uint64_t *)ctor++;
  }

  // call all global constructors
  while(entries--)
    (*ctor++)();
  
  // now everything is in place and we can invoke the kernel main() function
  main(0,0);

  /* We comply with the Itanium C++ ABI:
   *
   * Every constructor will registere its termination function by calling
   *    int __cxa_atexit(void (* f)(void *), void *p, void *d);
   * When finished, the termination function is called
   *    void __cxa_finalize(void *d);
   * Therefore we do not have to take care of destructors here.
   *
   * See src/bare-metal-generic/cxxabi.cpp and its documentation doc/cxxabi.txt
   */

  // not reached
  while(1);
}

#ifdef  __cplusplus
}
#endif

#endif // __USE_GLOBAL_CRT__

/* vim: set expandtab tabstop=2 shiftwidth=2 autoindent smartindent: */
