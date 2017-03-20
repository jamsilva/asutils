#define ASUTILS_PRIVATE_DEF
#include <as/fdio.h>
#include <as/string.h>

/* Private */

// These variables are thread-local for MT-Safety.
static __thread ulong iobuflen = 0;
static __thread char* iobuf = NULL;

static ulong maxdentlen = _SIZEOF(asdirent) + 256;

static int _(alloc_fdio)()
{
	if(iobuflen == 0)
	{
		ulong pagesize = _(getpagesize)();
		iobuflen = _ALIGN(pagesize + maxdentlen, pagesize);
		iobuf = (char*) _(anonmmap)(iobuflen);
		iobuflen -= maxdentlen;

		if(!iobuf)
		{
			errno = ENOMEM;
			return -1;
		}
	}

	return 0;
}

static long _(do_read)(int fd, char* buf, ulong len)
{
	long got;

	while((got = _(read)(fd, buf, len)) == -1 && errno == EINTR);

	return got;
}

static long _(do_write)(int fd, const char* buf, ulong len)
{
	long written;

	while((written = _(write)(fd, buf, len)) == -1 && errno == EINTR);

	return written;
}

/* Public API */

void _(cleanup_fdio)()
{
	if(iobuf)
		_(anonmunmap)(iobuf, iobuflen + maxdentlen);
}

int _(fgetc)(int fd)
{
	char c;
	ulong res = _(fread)(&c, (ulong) 1, (ulong) 1, fd);

	if(res == 0)
		return AS_EOF;

	return (int) c;
}

int _(fopen)(const char* path, const char* mode)
{
	int flags = 0;

	while(*mode)
		flags |= _(modeflags)(*mode++);

	return _(open)(path, _(normalizeflags)(flags));
}

int _(fputc)(int c, int fd)
{
	char ch = (char) c;
	return (int) _(do_write)(fd, &ch, (ulong) 1);
}

int _(fputs)(const char* str, int fd)
{
	return (int) _(do_write)(fd, str, _(strlen)(str));
}

int _(fprintf)(int fd, const char* fmt, ...)
{
	_TO_VA(ap, fmt,
		int res = _(vfprintf)(fd, fmt, ap);
	);
	return res;
}

ulong _(fread)(void* ptr, ulong len, ulong n, int fd)
{
	long got = _(do_read)(fd, (char*) ptr, len * n);
	
	if(got < 1)
		return 0;

	return (ulong) got;
}

int _(fseek)(int fd, long offset, int origin)
{
	return (int) _(lseek)(fd, offset, origin);
}

long _(ftell)(int fd)
{
	return _(fseek)(fd, 0, AS_SEEK_CUR);
}

ulong _(fwrite)(const void* ptr, ulong len, ulong n, int fd)
{
	long written = _(do_write)(fd, (const char*) ptr, len * n);

	if(written < 0)
		return 0;

	return (ulong) written;
}

int _(getc)(int fd)
{
	return _(fgetc)(fd);
}

int _(getchar)()
{
	return _(fgetc)(AS_STDIN);
}

int _(printf)(const char* fmt, ...)
{
	_TO_VA(ap, fmt,
		int res = _(vfprintf)(AS_STDOUT, fmt, ap);
	);
	return res;
}

int _(putc)(int c, int fd)
{
	return _(fputc)(c, fd);
}

int _(putchar)(int c)
{
	return _(fputc)(c, AS_STDOUT);
}

int _(puts)(const char* str)
{
	int res = _(fputs)(str, AS_STDOUT);

	if(res < 0)
		return res;

	_(putchar)('\n');
	return 0;
}

asdirent* _(readdir)(int dir)
{
	if(_(alloc_fdio)() == -1)
		return NULL;

	asdirent* dent = (asdirent*) &iobuf[iobuflen];
	long res = -1;
	ulong i;
	errno = EINVAL;

	for(i = _SIZEOF(asdirent) ; res == -1 && errno == EINVAL && i <= maxdentlen ; i += _SIZEOF(asdirent))
		res = _(getdents)(dir, dent, i);

	if(res == -1 && errno == EINVAL && i != maxdentlen)
		res = _(getdents)(dir, dent, maxdentlen);

	if(res < 1)
		return NULL;

	return dent;
}

int _(remove)(const char* path)
{
	int res = _(unlink)(path);

	if(res == -1 && errno == EISDIR)
		return _(rmdir)(path);

	return res;
}

int _(rename)(const char* path1, const char* path2)
{
	if(_(link)(path1, path2) == -1)
		return -1;

	if(_(remove)(path1) == -1)
	{
		int err = errno;
		_(remove)(path2);
		errno = err;
		return -1;
	}

	return 0;
}

void _(rewind)(int fd)
{
	_(fseek)(fd, 0, AS_SEEK_SET);
}

int _(vfprintf)(int fd, const char* fmt, va_list ap)
{
	if(_(alloc_fdio)() == -1)
		return -1;

	int res = _(vsnprintf)(iobuf, iobuflen, fmt, ap);

	if(res < 0)
		return res;

	ulong ures = (ulong) res;

	if(ures > iobuflen - 1)
	{
		errno = ENOMEM;
		return -1;
	}

	return (int) _(do_write)(fd, iobuf, ures);
}

int _(vprintf)(const char* fmt, va_list ap)
{
	return _(vfprintf)(AS_STDOUT, fmt, ap);
}
