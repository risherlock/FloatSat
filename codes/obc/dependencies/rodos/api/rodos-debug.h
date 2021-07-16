
#pragma once

/**
* @file rodos-debug.h
* @date 2008/06/17, 2017/jun
* @author Sergio Montenegro, Lutz Dittrich
* @brief debug and prinf functions 
*/

#include <stddef.h>
#include <stdint.h>

namespace RODOS {

/** To be used from printfs, to control screen poss , see http://www.termsys.demon.co.uk/vtansi.htm **/
#define SCREEN_CLEAR "\x1B[2J"
#define SCREEN_CLEAR_LINE "\x1B[K"
#define SCREEN_MOVE_CURSOR_TO  "\x1B[%d;%dH"
#define SCREEN_SCROLL_REGION  "\x1B[%d;%dr"
#define SCREEN_RESET  "\x1B[0m"

#define SCREEN_BLACK  "\x1B[30m"
#define SCREEN_RED    "\x1B[31m"
#define SCREEN_GREEN  "\x1B[32m"
#define SCREEN_YELLOW "\x1B[33m"
#define SCREEN_BLUE   "\x1B[34m"
#define SCREEN_WHITE  "\x1B[37m"
#define SCREEN_MAGENTA      "\x1b[35m"
#define SCREEN_CYAN         "\x1b[36m"
#define SCREEN_BRIGHT_RED   "\x1b[91m"
#define SCREEN_BRIGHT_GREEN "\x1b[92m"
#define SCREEH_UNDERLINE    "\x1b[4m"


// 3 macros are (really) required to be able to generate a string with the source code location like "debug.cpp : 23"
#define STRINGIZE(x) #x
#define NUMBER_TO_STRING(x) STRINGIZE(x)
#define LOCATION  __FILE__ " : " NUMBER_TO_STRING(__LINE__)


/// counts how many errors have been detected. Normally it shall be 0
extern unsigned long rodosErrorCounter;

/// register the error message of the last Error occurred: is is a unique pointer too
extern const char* rodosErrorMsg;
// extern unsigned long rodosErrorCode;


/** Writes 0 terminated  1/0 string representing the input 
 * value, takes the lower 'len' bits
 * warning: it writes in outbuf 'len' bytes + a terminating 0 (len +1)
 * @param[in]  val 4-byte to be represented as list of bits
 * @param[in]  len number of bits used within val
 * @param[in/out]  outBuf buffer to write string 
 * @return  outBuf buffer to write string
 */
char* formatBinary(uint32_t val, size_t len, char* outBuf);

/** Printf protected with a semaphore: Thread safe 
     Warning: %3.9f means 3 digits then 9 digits
 **/
void PRINTF(const char* fmt, ...) __attribute__((__format__(__printf__,1,2)));


void SPRINTF(char* dest, const char* fmt, ...) __attribute__((__format__(__printf__,2,3)));

size_t SNPRINTF(char* dest, size_t size, const char* fmt, ...) __attribute__((__format__(__printf__,3,4)));

/** id shall have only one bit set, prints only if (id & printfMask)  **/
void PRINTF_CONDITIONAL(uint32_t id, const char* fmt, ...) __attribute__((__format__(__printf__,2,3)));


/** Writes an error text with a leading title to stdout and keeps 
 * a counter. Used to report programming errors which shall be corrected.
 */
void RODOS_ERROR(const char* text);

/** Print a character bitwise, highest bit first. 
 *  obsolete! use %b in PRINTF
 *  @param[in]  val 8-bit value to be printed
 **/
//void printBin(unsigned char val); obsolete: use %b in PRINTF

/** Prints an array of bytes as hex values 
 *
 * @param[in] titel leading text in the line printed before values
 * @param[in] ptr pointer to array to be printed as unsigned char
 * @param[in] len number of characters, respectively bytes in the array
 */
void printCharsHex(const char* titel, void* ptr, size_t len);
void printCharsHex_Conditional(int negligibleLevel, const char* titel, void* ptr, size_t len);

/** Function to read a character from stream, in case one has been written.
 * Normal getchar (in linux) will stop the complete execution. 
 * In order to avoid waiting first isCharRaedy() is called.
 * The implementation is platform dependent defined in getcharnowait.cpp.
 *
 * @return latest character read, -1 if no character is available
 */
char getcharNoWait();

/** Checks if a new character has been received from stdin.
 * Associated to getcharNoWait().
 * Returns true if a new character is available in order to read it with getcharNoWait.
 *
 * @return 1 if a new character is in the buffer, 0 otherwise
 */
bool isCharReady();

/**
 * Get a string using getcharNoWait().
 * Returns If a string is ready it returns a pointer to it, else it returns 0 (no wait).
 * The maximal length of a string is 120 chars (due to internal buffers).
 * The String will not be modified until the next call
 * to getsNoWait.
 * Not Thread save. Only one reader from stdin is allowed.
 * Else... I don't know, try what it does and let me know.
 */
char* getsNoWait();

/**
 * if you activate the topic cat input, all input string from
 * terminal will be distributed to the topic charInput
 * until you call a getcharNoWait() or getsNoWait();
 */

void activateTopicCharInput();

/**
 * Like in linux fflush(stdout)
 */
void FFLUSH();


/** Set to true or false to allow RODOS-ERROR(...) To print text
 ** if false RODOS_ERROR(...) will not print, only registered in errLog **/
extern bool printErrorReports;

} // namespace 

/** Sets the pirntf mask for PRINTF_CONDITIONAL, 0 means no printf (not even from PRINTF)
 ** else see tutorials/core/printfconditional 
 ** intentionally out of name spaces ***/

extern uint32_t printfMask;

