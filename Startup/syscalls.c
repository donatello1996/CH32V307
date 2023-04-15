#include <sys/stat.h>

void *_sbrk(ptrdiff_t incr) {
    extern char _end[];
    extern char _heap_end[];
    static char *curbrk = _end;

    void *ret;

    if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
    return NULL - 1;

    curbrk += incr;
    ret = curbrk - incr;

    return ret;
}


int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;

  return 0;
}

int _close(int file) {
  return -1;
}

int _isatty(int file) {
  return -1;
}

off_t _lseek(int file, off_t pos, int whence) {
  return -1;
}

int _read(int file) {
  return -1;
}