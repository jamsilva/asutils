#include <sys/prctl.h>
#include <sys/socket.h>

#define ASUTILS_PRIVATE_DEF
#include <as/fdio.h>
#include <as/utils.h>
#include <as/string.h>

/* Private */

// These variables are thread-local for MT-Safety.
static __thread char  tnamebuf[16];
static __thread long* tlist = NULL;

static long* _(alloc_task)()
{
	if(!tlist)
		tlist = (long*) _(anonmmap)(_(getpagesize)());

	return tlist;
}

static long* _(gettids_thread_aware)(long pid)
{
	long* tlistptr = _(alloc_task)();

	if(!tlistptr)
		return NULL;

	// strlen("/proc/") + sizeof(long) * max_decimals_per_byte + strlen("/task") + '\0'.
	char taskdir[6 + _SIZEOF(long) * 3 + 5 + 1];
	_(sprintf)(taskdir, "/proc/%ld/task", pid);
	int tasks = _(opendir)(taskdir);

	if(tasks == -1)
		return NULL;

	while(1)
	{
		asdirent* next_task = _(readdir)(tasks);

		// No more direntries.
		if(!next_task)
			break;

		const char* tidstr = _(strrchr)(next_task->name, '/');

		if(!tidstr)
			tidstr = next_task->name;

		long tid = (long) _(strtoll)(tidstr, 10);

		// Found a thread of pid.
		if(tid > 0)
			*tlistptr++ = tid;
	}

	_(close)(tasks);
	*tlistptr = (long) 0;
	return tlist;
}

static long* _(gettids_fallback)(long pid)
{
	long* tlistptr = _(alloc_task)();

	if(!tlistptr)
		return NULL;

	// strlen("/proc/") + sizeof(long) * max_decimals_per_byte + strlen("/fd/") + sizeof(int) * max_decimals_per_byte + '\0'.
	char path[6 + _SIZEOF(long) * 3 + 4 + _SIZEOF(int) * 3 + 1];
	int procs = _(opendir)("/proc");

	if(procs == -1)
		return NULL;

	SAFE(POSIX.1-2001) int marker = socket(PF_UNIX, SOCK_RAW, 0);

	if(marker == -1)
		return NULL;

	_(sprintf)(path, "/proc/%ld/fd/%d", pid, marker);
	long inode = _(getinode)(path);

	if(inode == -1)
	{
		_(close)(marker);
		return NULL;
	}

	while(1)
	{
		asdirent* next_proc = _(readdir)(procs);

		// No more direntries.
		if(!next_proc)
			break;

		const char* tidstr = _(strrchr)(next_proc->name, '/');

		if(!tidstr)
			tidstr = next_proc->name;

		long tid = (long) _(strtoll)(tidstr, 10);

		// Not a tid.
		if(tid < 1)
			continue;

		// Optimization - tid is a thread of itself.
		if(tid == pid)
		{
			*tlistptr++ = tid;
			continue;
		}

		_(sprintf)(path, "/proc/%s/fd/%d", tidstr, marker);

		// Same inode - this is a thread of pid.
		if(inode == _(getinode)(path))
			*tlistptr++ = tid;
	}

	_(close)(marker);
	_(close)(procs);
	*tlistptr = (long) 0;
	return tlist;
}

/* Public API */

void _(cleanup_task)()
{
	if(tlist)
		_(anonmunmap)(tlist, _(getpagesize)());
}

long _(gettid)()
{
	SAFE(SYSCALL) return syscall(SYS_gettid);
}

long* _(gettids)(long pid)
{
	long* list = _(gettids_thread_aware)(pid);

	if(list && list[0] && list[1]) // Found at least two threads.
		return list;

	return _(gettids_fallback)(pid);
}

char* _(gettname)()
{
	SAFE(SYSCALL) prctl(PR_GET_NAME, tnamebuf);
	return tnamebuf;
}

void _(settname)(const char* name)
{
	SAFE(SYSCALL) prctl(PR_SET_NAME, name);
}

void _(texit)(int ret)
{
	SAFE(SYSCALL) syscall(SYS_exit, ret);
}

int _(tkill)(long tid, int signum)
{
	SAFE(SYSCALL) return (int) syscall(SYS_tkill, (pid_t) tid, signum);
}
