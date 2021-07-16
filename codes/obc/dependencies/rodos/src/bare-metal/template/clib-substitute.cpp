/*
 * Clib substitute template
 *
 *  Created on: 13.03.2012
 *  Michael Ruffer
 */


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

int _close(int file) {
    (void)file;
    return -1;
}
int _execve(char* name, char** argv, char** env) {
    (void)name;
    (void)argv;
    (void)env;
    return -1;
}
int _fork(void) {
    return -1;
}
int _fstat(int file, struct stat* st) {
    (void)file;
    (void)st;
    return 0;
}
int _getpid(void) {
    return 1;
}
int _isatty(int file) {
    (void)file;
    return 1;
}
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1;
}
int _link(char* old, char* pNew) {
    (void)old;
    (void)pNew;
    return -1;
}
int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}
int _open(const char* name, int flags, int mode) {
    (void)name;
    (void)flags;
    (void)mode;
    return -1;
}
int _read(int file, char* ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}
int _write(int file, char* ptr, int len) {
    (void)file;
    (void)ptr;
    return len;
}
void* _sbrk(int incr) {
    (void)incr;
    return 0;
}

} // end extern "C"
