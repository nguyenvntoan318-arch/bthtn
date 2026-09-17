#include "main.h"
#include <stdio.h>

/* Fix lỗi System Calls cho Newlib Nano */
#include <sys/stat.h>

__attribute__((weak)) int _isatty(int fd) { return 1; }
__attribute__((weak)) int _close(int fd) { return -1; }
__attribute__((weak)) int _lseek(int fd, int ptr, int dir) { return 0; }
__attribute__((weak)) int _fstat(int fd, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
__attribute__((weak)) int _read(int fd, char *ptr, int len) { return 0; }
__attribute__((weak)) int _write(int fd, char *ptr, int len) { return len; }

caddr_t _sbrk(int incr) {
    extern char end;
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &end;
    }
    prev_heap_end = heap_end;
    heap_end += incr;

    return (caddr_t) prev_heap_end;
}
