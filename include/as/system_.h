#ifndef AS_SYSTEM_H_INCLUDED_
#define AS_SYSTEM_H_INCLUDED_
#include <as/types/dirent.h>
#include <as/types/tname.h>

ASUTILS_BEGIN_CXXCOMPAT

void* as_anonmmap      (ulong len);
void  as_anonmunmap    (void* addr, ulong len);
int   as_close         (int fd);
long  as_getdents      (int dirfd, as_dirent_t* buf, ulong len);
long  as_getinode      (const char* path);
ulong as_getpagesize   (void);
long  as_getpid        (void);
long  as_gettid        (void);
ulong as_gettids       (long* tids, ulong size);
void  as_gettname      (as_tname_t name);
int   as_link          (const char* path1, const char* path2);
long  as_lseek         (int fd, long offset, int mode);
int   as_modeflags     (char mode);
int   as_normalizeflags(int flags);
int   as_open          (const char* path, int flags);
int   as_opendir       (const char* path);
void  as_pexit         (int ret);
long  as_read          (int fd, void* buf, ulong len);
long  as_readlink      (const char* path, char* buf, ulong len);
int   as_rmdir         (const char* path);
void  as_settname      (const as_tname_t name);
void  as_texit         (int ret);
ulong as_timesecs      (void);
int   as_tkill         (long tid, int signum);
int   as_unlink        (const char* path);
void  as_waitexit      (long pid);
long  as_write         (int fd, const void* buf, ulong len);

ASUTILS_END_CXXCOMPAT

#endif
