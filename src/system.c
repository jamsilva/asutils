#define SAFE(standard)

#ifdef __linux__
	#define _GNU_SOURCE 1
	#include <unistd.h>
	#include <sys/syscall.h>
	
	#include "system/linux/file.c"
	#include "system/linux/thread.c"
	#include "system/linux/trace.c"
	
	void _(cleanup_system)()
	{
		_(cleanup_task)();
	}
#endif

#if !defined(_WIN32) && !defined(__WIN32__)
	#include "system/posix/pinfo.c"
	#include "system/posix/signal.c"
	#include "system/posix/system.c"
	#include "system/posix/time.c"
#endif
