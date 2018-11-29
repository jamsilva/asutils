#include <as/private_.h>

/* Public API */

long as_getdents(int dirfd, as_dirent_t* storage, ulong len)
{
	SAFE(SYSCALL) long res = syscall(SYS_getdents, (uint) dirfd, storage, (uint) len);
	storage->namelen -= 1 + (((ulong) &storage->name) - ((ulong) storage));
	return res;
}
