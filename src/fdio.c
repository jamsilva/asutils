/* System */
#include <errno.h>

#include <as/private_.h>

/* Private */

static long as_do_read(int fd, char* buf, ulong len)
{
	long got;

	while((got = as_read(fd, buf, len)) == -1 && errno == EINTR);

	return got;
}

static long as_do_write(int fd, const char* buf, ulong len)
{
	long written;

	while((written = as_write(fd, buf, len)) == -1 && errno == EINTR);

	return written;
}

/* Public API */

int as_fgetc(int fd)
{
	char c;
	ulong res = as_fread(&c, (ulong) 1, (ulong) 1, fd);

	if(res == 0)
		return AS_EOF;

	return (int) c;
}

char* as_fgets(char* str, ulong len, int fd)
{
	if(!str || !len)
		return NULL;

	char* at = str;

	while(--len)
	{
		int c = as_fgetc(fd);

		if(c == AS_EOF)
		{
			if(at == str)
				return NULL;

			break;
		}

		*at++ = (char) c;

		if(c == '\n')
			break;
	}

	*at = '\0';
	return str;
}

int as_fopen(const char* path, const char* mode)
{
	int flags = 0;

	while(*mode)
		flags |= as_modeflags(*mode++);

	return as_open(path, as_normalizeflags(flags));
}

int as_fputc(int c, int fd)
{
	char ch = (char) c;
	return (int) as_do_write(fd, &ch, (ulong) 1);
}

int as_fputs(const char* str, int fd)
{
	return (int) as_do_write(fd, str, as_strlen(str));
}

int as_fprintf(int fd, const char* fmt, ...)
{
	TO_VA(ap, fmt,
		int res = as_vfprintf(fd, fmt, ap);
	);
	return res;
}

ulong as_fread(void* ptr, ulong len, ulong n, int fd)
{
	long got = as_do_read(fd, (char*) ptr, len * n);

	if(got < 1)
		return 0;

	return (ulong) got;
}

int as_fseek(int fd, long offset, int origin)
{
	return (int) as_lseek(fd, offset, origin);
}

long as_ftell(int fd)
{
	return as_fseek(fd, 0, AS_SEEK_CUR);
}

ulong as_fwrite(const void* ptr, ulong len, ulong n, int fd)
{
	long written = as_do_write(fd, (const char*) ptr, len * n);

	if(written < 0)
		return 0;

	return (ulong) written;
}

int as_getc(int fd)
{
	return as_fgetc(fd);
}

int as_getchar()
{
	return as_fgetc(AS_STDIN);
}

int as_printf(const char* fmt, ...)
{
	TO_VA(ap, fmt,
		int res = as_vfprintf(AS_STDOUT, fmt, ap);
	);
	return res;
}

int as_putc(int c, int fd)
{
	return as_fputc(c, fd);
}

int as_putchar(int c)
{
	return as_fputc(c, AS_STDOUT);
}

int as_puts(const char* str)
{
	int res = as_fputs(str, AS_STDOUT);

	if(res < 0)
		return res;

	as_putchar('\n');
	return 0;
}

int as_readdir(int dir, as_dirent_t* dirent)
{
	errno = EINVAL;

	if(!dirent)
		return -1;

	long res = -1;
	ulong maxdirent = sizeof(as_dirent_t);
	ulong mindirent = maxdirent - AS_MAXDIRLEN + 1;
	ulong i = mindirent;

	for( ; res == -1 && errno == EINVAL && i <= maxdirent ; i += mindirent)
		res = as_getdents(dir, dirent, i);

	if(res == -1 && errno == EINVAL && i != maxdirent)
		res = as_getdents(dir, dirent, maxdirent);

	if(res < 1)
		return -1;

	return 0;
}

int as_remove(const char* path)
{
	int res = as_unlink(path);

	if(res == -1 && errno == EISDIR)
		return as_rmdir(path);

	return res;
}

int as_rename(const char* path1, const char* path2)
{
	if(as_link(path1, path2) == -1)
		return -1;

	if(as_remove(path1) == -1)
	{
		int err = errno;
		as_remove(path2);
		errno = err;
		return -1;
	}

	return 0;
}

void as_rewind(int fd)
{
	as_fseek(fd, 0, AS_SEEK_SET);
}

int as_vfprintf(int fd, const char* fmt, va_list ap)
{
	ulong iobuflen = as_getpagesize();
	char* iobuf = (char*) as_anonmmap(iobuflen);
	int res = as_vsnprintf(iobuf, iobuflen, fmt, ap);

	if(res < 0)
		return res;

	ulong ures = (ulong) res;

	if(ures > iobuflen - 1)
	{
		errno = ENOMEM;
		as_anonmunmap(iobuf, iobuflen);
		return -1;
	}

	res = (int) as_do_write(fd, iobuf, ures);
	as_anonmunmap(iobuf, iobuflen);
	return res;
}

int as_vprintf(const char* fmt, va_list ap)
{
	return as_vfprintf(AS_STDOUT, fmt, ap);
}
