#include "yprintf.h"

/**
 * MAIN() is the entry point for user code.
 * This is dummy code and should be overwritten by user code.
 * If the user provides no MAIN, then we take this default.
 */

void MAIN() __attribute__((weak));
void MAIN() { RODOS::xprintf("Default internal MAIN\n"); }


