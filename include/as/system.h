#ifndef AS_SYSTEM_H_INCLUDED_
#define AS_SYSTEM_H_INCLUDED_

#define AS_ATTACH 0
#define AS_DETACH 1

typedef struct
{
	ulong  inode;
	ulong  offset;
	ushort namelen;
	char   name[1];
} asdirent;

ASUTILS_BEGIN_CXXCOMPAT

#include <errno.h>

void* ASUTILS_FUNC(anonmmap)      (ulong len);
void  ASUTILS_FUNC(anonmunmap)    (void* addr, ulong len);
int   ASUTILS_FUNC(close)         (int fd);
long  ASUTILS_FUNC(getdents)      (int dirfd, asdirent* buf, ulong len);
long  ASUTILS_FUNC(getinode)      (const char* path);
ulong ASUTILS_FUNC(getpagesize)   (void);
long  ASUTILS_FUNC(getpid)        (void);
long  ASUTILS_FUNC(gettid)        (void);
long* ASUTILS_FUNC(gettids)       (long pid);
char* ASUTILS_FUNC(gettname)      (void);
int   ASUTILS_FUNC(link)          (const char* path1, const char* path2);
long  ASUTILS_FUNC(lseek)         (int fd, long offset, int mode);
int   ASUTILS_FUNC(modeflags)     (char mode);
int   ASUTILS_FUNC(normalizeflags)(int flags);
int   ASUTILS_FUNC(open)          (const char* path, int flags);
int   ASUTILS_FUNC(opendir)       (const char* path);
int   ASUTILS_FUNC(openmarker)    (void);
void  ASUTILS_FUNC(pexit)         (int ret);
long  ASUTILS_FUNC(read)          (int fd, void* buf, ulong len);
long  ASUTILS_FUNC(readlink)      (const char* path, char* buf, ulong len);
int   ASUTILS_FUNC(rmdir)         (const char* path);
void  ASUTILS_FUNC(settname)      (const char* name);
void  ASUTILS_FUNC(texit)         (int ret);
ulong ASUTILS_FUNC(timesecs)      (void);
int   ASUTILS_FUNC(tkill)         (long tid, int signum);
int   ASUTILS_FUNC(unlink)        (const char* path);
void  ASUTILS_FUNC(waitexit)      (long pid);
long  ASUTILS_FUNC(write)         (int fd, const void* buf, ulong len);

ASUTILS_END_CXXCOMPAT

#endif
