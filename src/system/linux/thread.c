#include <errno.h>
#include <sys/prctl.h>
#include <sys/socket.h>

#include <as/private_.h>

/* Private */

static ulong as_gettids_modern(long pid, long* tids, ulong size)
{
	// strlen("/proc/") + sizeof(long) * max_decimals_per_byte + strlen("/task") + '\0'.
	char taskdir[6 + sizeof(long) * 3 + 5 + 1];
	as_sprintf(taskdir, "/proc/%ld/task", pid);
	int tasks = as_opendir(taskdir);
	ulong count = 1;

	if(tasks == -1)
		return count;

	as_dirent_t next_task;

	while(1)
	{
		if(as_readdir(tasks, &next_task) == -1) // No more direntries.
			break;

		const char* tidstr = as_strrchr(next_task.name, '/');

		if(!tidstr)
			tidstr = next_task.name;

		long tid = (long) as_strtoll(tidstr, 10);

		if(tid == pid) // The pid is already there.
			continue;

		if(tid > 0) // Found a thread of pid.
		{
			if(count < size)
				tids[count] = tid;

			++count;
		}
	}

	as_close(tasks);
	return count;
}

static ulong as_gettids_linuxthreads(long pid, long* tids, ulong size)
{
	// strlen("/proc/") + sizeof(long) * max_decimals_per_byte + strlen("/fd/") + sizeof(int) * max_decimals_per_byte + '\0'.
	char path[6 + sizeof(long) * 3 + 4 + sizeof(int) * 3 + 1];
	int procs = as_opendir("/proc");
	ulong count = 1;
	utiny skipped_manager = 0;

	if(procs == -1)
		return count;

	SAFE(POSIX.1-2001) int marker = socket(PF_UNIX, SOCK_RAW, 0);

	if(marker == -1)
		return count;

	as_sprintf(path, "/proc/%ld/fd/%d", pid, marker);
	long inode = as_getinode(path);

	if(inode == -1)
	{
		as_close(marker);
		return count;
	}

	as_dirent_t next_proc;

	while(1)
	{
		if(as_readdir(procs, &next_proc) == -1) // No more direntries.
			break;

		const char* tidstr = as_strrchr(next_proc.name, '/');

		if(!tidstr)
			tidstr = next_proc.name;

		long tid = (long) as_strtoll(tidstr, 10);

		if(tid < 1) // Not a tid.
			continue;

		if(tid == pid) // The pid is already there.
			continue;

		as_sprintf(path, "/proc/%s/fd/%d", tidstr, marker);

		if(inode == as_getinode(path)) // Same inode - this is a thread of pid.
		{
			if(!skipped_manager)
			{
				skipped_manager = 1;
				continue;
			}

			if(count < size)
				tids[count] = tid;

			++count;
		}
	}

	as_close(marker);
	as_close(procs);
	return count;
}

/* Public API */

long as_gettid()
{
	SAFE(SYSCALL) return syscall(SYS_gettid);
}

ulong as_gettids(long* tids, ulong size)
{
	long pid = as_getpid();

	if(!tids)
		size = 0;

	if(size > 0)
		tids[0] = pid;

	ulong count = as_gettids_modern(pid, tids, size);

	if(count != 1)
		return count;

	return as_gettids_linuxthreads(pid, tids, size);
}

void as_gettname(as_tname_t name)
{
	if(!name)
		errno = EINVAL;
	else
		SAFE(SYSCALL) prctl(PR_GET_NAME, name);
}

void as_settname(const as_tname_t name)
{
	SAFE(SYSCALL) prctl(PR_SET_NAME, name);
}

void as_texit(int ret)
{
	SAFE(SYSCALL) syscall(SYS_exit, ret);
}

int as_tkill(long tid, int signum)
{
	SAFE(SYSCALL) return (int) syscall(SYS_tkill, (pid_t) tid, signum);
}
