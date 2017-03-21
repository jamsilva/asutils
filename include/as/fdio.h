#ifndef AS_FDIO_H_INCLUDED_
#define AS_FDIO_H_INCLUDED_
#include <as/utils.h>

#define AS_EOF -1

#define AS_STDIN  0
#define AS_STDOUT 1
#define AS_STDERR 2

#define AS_SEEK_SET 0
#define AS_SEEK_CUR 1
#define AS_SEEK_END 2

ASUTILS_BEGIN_CXXCOMPAT

#include <stdarg.h>

int       ASUTILS_FUNC(fgetc)   (int fd);
char*     ASUTILS_FUNC(fgets)   (char* str, ulong len, int fd);
int       ASUTILS_FUNC(fopen)   (const char* path, const char* mode);
int       ASUTILS_FUNC(fputc)   (int c, int fd);
int       ASUTILS_FUNC(fputs)   (const char* str, int fd);
int       ASUTILS_FUNC(fprintf) (int fd, const char* fmt, ...);
ulong     ASUTILS_FUNC(fread)   (void* ptr, ulong len, ulong n, int fd);
int       ASUTILS_FUNC(fseek)   (int fd, long offset, int origin);
long      ASUTILS_FUNC(ftell)   (int fd);
ulong     ASUTILS_FUNC(fwrite)  (const void* ptr, ulong len, ulong n, int fd);
int       ASUTILS_FUNC(getc)    (int fd);
int       ASUTILS_FUNC(getchar) (void);
int       ASUTILS_FUNC(printf)  (const char* fmt, ...);
int       ASUTILS_FUNC(putc)    (int c, int fd);
int       ASUTILS_FUNC(putchar) (int c);
int       ASUTILS_FUNC(puts)    (const char* str);
asdirent* ASUTILS_FUNC(readdir) (int dir);
int       ASUTILS_FUNC(remove)  (const char* path);
int       ASUTILS_FUNC(rename)  (const char* path1, const char* path2);
void      ASUTILS_FUNC(rewind)  (int fd);
int       ASUTILS_FUNC(vfprintf)(int fd, const char* fmt, va_list ap);
int       ASUTILS_FUNC(vprintf) (const char* fmt, va_list ap);

ASUTILS_END_CXXCOMPAT

#endif
