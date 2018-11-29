#define SAFE(standard)

#ifdef __linux__
	#define _GNU_SOURCE 1
	#include <signal.h>
	#include <unistd.h>
	#include <sys/syscall.h>

	#include "system/linux/file.c"
	#include "system/linux/thread.c"
	#include "system/linux/trace.c"
#endif

#if !defined(_WIN32) && !defined(__WIN32__)
	#include "system/posix/pinfo.c"
	#include "system/posix/signal.c"
	#include "system/posix/system.c"
#endif
