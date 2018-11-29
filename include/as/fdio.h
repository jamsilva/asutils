#ifndef AS_FDIO_H_INCLUDED_
#define AS_FDIO_H_INCLUDED_
#include <as/utils.h>
#include <as/types/dirent.h>

#define AS_EOF -1

#define AS_STDIN  0
#define AS_STDOUT 1
#define AS_STDERR 2

#define AS_SEEK_SET 0
#define AS_SEEK_CUR 1
#define AS_SEEK_END 2

ASUTILS_BEGIN_CXXCOMPAT

#include <stdarg.h>

int   as_fgetc   (int fd);
char* as_fgets   (char* str, ulong len, int fd);
int   as_fopen   (const char* path, const char* mode);
int   as_fputc   (int c, int fd);
int   as_fputs   (const char* str, int fd);
int   as_fprintf (int fd, const char* fmt, ...);
ulong as_fread   (void* ptr, ulong len, ulong n, int fd);
int   as_fseek   (int fd, long offset, int origin);
long  as_ftell   (int fd);
ulong as_fwrite  (const void* ptr, ulong len, ulong n, int fd);
int   as_getc    (int fd);
int   as_getchar (void);
int   as_printf  (const char* fmt, ...);
int   as_putc    (int c, int fd);
int   as_putchar (int c);
int   as_puts    (const char* str);
int   as_readdir (int dir, as_dirent_t* dirent);
int   as_remove  (const char* path);
int   as_rename  (const char* path1, const char* path2);
void  as_rewind  (int fd);
int   as_vfprintf(int fd, const char* fmt, va_list ap);
int   as_vprintf (const char* fmt, va_list ap);

ASUTILS_END_CXXCOMPAT

#endif
