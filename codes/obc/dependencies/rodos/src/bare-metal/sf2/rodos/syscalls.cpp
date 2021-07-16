/**
 * syscalls.cpp
 *
 *  Created on: 13.03.2012
 *      Author: Michael Ruffer
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#undef errno
extern int errno;

#include "hal/hal_uart.h"
#include "default-platform-parameter.h"
#include "misc-rodos-funcs.h"
#include "rodos-debug.h"
#include "hw_specific.h"

namespace RODOS {

#define T2H_SYSCALL_INIT_ARGV 14

extern HAL_UART uart_stdout;

extern "C" {

/* newlib stubs
 * The following function are necessary for newlib. It is very important
 * that there are valid implementations for _sbrk_r and sbrk. These
 * functions are used for memory allocation.
 * For more informations about syscalls visit:
 * http://sourceware.org/newlib/libc.html#Syscalls
 * http://sourceware.org/newlib/libc.html#Reentrant-Syscalls
 *
 */
#if 1
int _close(int file);
int _close(int file) {
    (void)file;
    return -1;
}

char*  __env[1] = { 0 };
char** environ  = __env;

int _execve(char* name, char** argv, char** env);
int _execve(char* name, char** argv, char** env) {
    (void)name;
    (void)argv;
    (void)env;
    errno = ENOMEM;
    return -1;
}

int _fork(void);
int _fork(void) {
    errno = EAGAIN;
    return -1;
}

int _fstat(int file, struct stat* st);
int _fstat(int file, struct stat* st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid(void);
int _getpid(void) {
    return 1;
}

int _isatty(int file);
int _isatty(int file) {
    (void)file;
    return 1;
}

int _kill(int pid, int sig);
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _link(char* old, char* pNew);
int _link(char* old, char* pNew) {
    (void)old;
    (void)pNew;
    errno = EMLINK;
    return -1;
}

int _lseek(int file, int ptr, int dir);
int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _open(const char* name, int flags, int mode);
int _open(const char* name, int flags, int mode) {
    (void)name;
    (void)flags;
    (void)mode;
    return -1;
}

int _read(int file, char* ptr, int len);
int _read(int file, char* ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

register char* stack_ptr asm("sp");
extern char    _estack;         // see linkerscript in \rodos-core\src\bare-metal\sf2\CMSIS\startup_gcc
extern char    _Min_Stack_Size; // see linkerscript in \rodos-core\src\bare-metal\sf2\CMSIS\startup_gcc
caddr_t        _sbrk(size_t incr);
caddr_t        _sbrk(size_t incr) {
    extern char  __heap_start__ asm("__heap_start__"); /* Defined by the linker.  */
    static char* heap_end;
    char*        prev_heap_end;

    if(heap_end == NULL)
        heap_end = &__heap_start__;

    prev_heap_end = heap_end;

    // Test, ob stack und heap kollidieren funktioniert in urspruenglicher
    // implementierung nicht, weil
    // RODOS den Stack verwaltet und der Stackpointer adressmaessig
    // irgendwo im XMALLOC-Puffer liegt. Der Puffer liegt z.Z. an einer
    // Adresse die immer kleiner als heap_end ist -> vgl. Linkerscript!!!
    // Der Hauptstack (MSP) wird (nachdem RODOS gestartet ist) nur noch
    // von ISRs & dem Scheduler verwendet.
    //if (heap_end + incr > stack_ptr) {
    if(heap_end + incr > &_estack - _Min_Stack_Size) {
        /* Some of the libstdc++-v3 tests rely upon detecting
		 out of memory errors, so do not abort here.  */
#    if 0
		extern void abort (void);

		_write (1, "_sbrk: Heap and stack collision\n", 32);

		abort ();
#    else
        asm("bkpt;");
        errno = ENOMEM;
        return (caddr_t)-1;
#    endif
    }

    heap_end += incr;

    return (caddr_t)prev_heap_end;
}

/*
 int stat(char *file, struct stat *st) {
 st->st_mode = S_IFCHR;
 return 0;
 }
 */

int _times(struct tms* buf);
int _times(struct tms* buf) {
    (void)buf;
    return -1;
}

int _unlink(char* name);
int _unlink(char* name) {
    (void)name;
    errno = ENOENT;
    return -1;
}

int _wait(int* status);
int _wait(int* status) {
    (void)status;
    errno = ECHILD;
    return -1;
}

int putchar(int ic);

int _write(int file, const char* ptr, size_t len);
int _write(int file, const char* ptr, size_t len) {
    (void)file;
    size_t todo;

    for(todo = 0; todo < len; todo++) {

        putchar(*ptr);
        ptr++;
    }
    return static_cast<int>(len);
}

/*
 * reentrant syscalls
 */
long _write_r(void* reent, int fd, const void* buf, size_t cnt);
long _write_r(void* reent, int fd, const void* buf, size_t cnt) {
    (void)reent;
    return _write(fd, (const char*)buf, cnt);
}

caddr_t _sbrk_r(void* reent, size_t incr);
caddr_t _sbrk_r(void* reent, size_t incr) {
    (void)reent;
    return _sbrk(incr);
}
#endif
/* end newlib stubs */

//puts chars
int putchar(int ic) {
    uint8_t c = (uint8_t)(ic & 0xff);

    if(c == '\n') {
        putchar('\r');
    }

    while(uart_stdout.putcharNoWait(c) == -1) {
        if(isSchedulerRunning()) {
            uart_stdout.suspendUntilWriteFinished();
        } else {
            while(!uart_stdout.isWriteFinished()) {}
        }
    }

    return c;
}

//puts strings
int puts(const char* str);
int puts(const char* str) {
    const char* c;
    c = str;
    while(*c) {
        putchar(*c++);
    }
    return 0;
}

void abort(void);
void abort(void) {
    while(1)
        ;
}

int sched_yield();
int sched_yield();
} // end extern "C"

void sp_partition_yield() {}
void enterSleepMode() {}


void FFLUSH() {}

} /* namespace RODOS */
