/* System */
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define ASUTILS_PRIVATE_DEF
#include <as/fdio.h>
#include <as/string.h>
#include <as/utils.h>

#ifndef MAP_ANONYMOUS
	#define MAP_ANONYMOUS MAP_ANON
#endif

/* Public API */

void* _(anonmmap)(ulong len)
{
	SAFE(SYSCALL) return mmap(NULL, (size_t) len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, (off_t) 0);
}

void _(anonmunmap)(void* addr, ulong len)
{
	SAFE(SYSCALL) munmap(addr, (size_t) len);
}

int _(close)(int fd)
{
	SAFE(POSIX.1-2001) return close(fd);
}

long _(getinode)(const char* path)
{
	struct stat sstat;
	SAFE(POSIX.1-2001) int res = stat(path, &sstat);

	if(res == -1)
		return (long) -1;

	return (long) sstat.st_ino;
}

ulong _(getpagesize)()
{
	SAFE(POSIX.1-2001) long pagesize = sysconf(_SC_PAGESIZE);

	if(pagesize <= 0)
		return (ulong) 4096;

	return (ulong) pagesize;
}

long _(getpid)()
{
	SAFE(POSIX.1-2001) return (long) getpid();
}

int _(link)(const char* path1, const char* path2)
{
	SAFE(POSIX.1-2001) return link(path1, path2);
}

long _(lseek)(int fd, long offset, int mode)
{
	SAFE(POSIX.1-2001) return (long) lseek(fd, (off_t) offset, mode);
}

int _(modeflags)(char mode)
{
	#ifndef O_CLOEXEC
		#define O_CLOEXEC 0
	#endif
	
	switch(mode)
	{
		#define mode_to_flags(m, f) case m: return f;
		mode_to_flags('r', O_RDONLY);
		mode_to_flags('w', O_CREAT  | O_TRUNC | O_WRONLY);
		mode_to_flags('a', O_APPEND | O_CREAT | O_WRONLY);
		mode_to_flags('+', O_RDWR);
		mode_to_flags('x', O_EXCL);
		mode_to_flags('e', O_CLOEXEC);
	}

	return mode;
}

int _(normalizeflags)(int flags)
{
	if(flags & O_RDWR)
		return flags & ~(O_RDONLY | O_WRONLY);

	return flags;
}

int _(open)(const char* path, int flags)
{
	SAFE(POSIX.1-2001) return open(path, flags);
}

int _(opendir)(const char* path)
{
	SAFE(POSIX.1-2001) return open(path, O_RDONLY | O_DIRECTORY);
}

void _(pexit)(int ret)
{
	SAFE(POSIX.1-2001) _exit(ret);
}

long _(read)(int fd, void* buf, ulong len)
{
	SAFE(POSIX.1-2001) return (long) read(fd, buf, (size_t) len);
}

long _(readlink)(const char* path, char* buf, ulong len)
{
	SAFE(POSIX.1-2001) return (long) readlink(path, buf, (size_t) len);
}

int _(rmdir)(const char* path)
{
	SAFE(POSIX.1-2001) return rmdir(path);
}

ulong _(timesecs)()
{
	struct timespec ts;
	SAFE(POSIX.1-2001) clock_gettime(CLOCK_REALTIME, &ts);
	return (ulong) ts.tv_sec;
}

int _(unlink)(const char* path)
{
	SAFE(POSIX.1-2001) return unlink(path);
}

void _(waitexit)(long pid)
{
	SAFE(POSIX.1-2001) waitpid((pid_t) pid, NULL, WEXITED);
}

long _(write)(int fd, const void* buf, ulong len)
{
	SAFE(POSIX.1-2001) return (long) write(fd, buf, (size_t) len);
}
