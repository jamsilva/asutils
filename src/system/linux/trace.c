#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include <as/private_.h>

/* Private */

typedef struct
{
	void*  begin;
	void*  end;
	void*  adjptr;
	ushort namelen;
	char   name[256];
} aslibsyminfo;

static int tracefd;
static volatile long* volatile tids;

static aslibsyminfo* _(getsyminfo)(void* ptr, aslibsyminfo* syminfo)
{
	char localbuf[512];
	syminfo->adjptr = ptr;

	// strlen("/proc/") + sizeof(long) * max_decimals_per_byte + strlen("/task/") + sizeof(long) * max_decimals_per_byte + strlen("/maps") + '\0'.
	char mapdir[6 + sizeof(long) * 3 + 6 + sizeof(long) * 3 + 5 + 1];
	_(sprintf)(mapdir, "/proc/%ld/task/%ld/maps", _(getpid)(), _(gettid)());

	int fd = _(fopen)(mapdir, "r");

	if(fd == -1)
	{
		_(sprintf)(mapdir, "/proc/%ld/maps", _(gettid)());
		fd = _(fopen)(mapdir, "r");
	}

	if(fd == -1)
		return NULL;

	while(1)
	{
		char* line = _(fgets)(localbuf, sizeof(localbuf), fd);

		if(!line)
		{
			_(close)(fd);
			return NULL;
		}

		syminfo->begin = (void*) ((ulong) _(strtoll)(line, 16));

		if(ptr < syminfo->begin)
			continue;

		const char* at = _(strchr)(line, '-');

		if(!at)
			continue;

		syminfo->end = (void*) ((ulong) _(strtoll)(++at, 16));

		if(ptr > syminfo->end)
			continue;

		utiny spaces_to_skip = 5;

		while(spaces_to_skip)
		{
			if(*at == '\0')
				break;

			if(*++at == ' ')
				--spaces_to_skip;
		}

		if(*at != ' ')
			continue;

		while(*++at == ' ');

		syminfo->namelen = (ushort) (_(stpncpy)(syminfo->name, at, sizeof(syminfo->name)) - syminfo->name);

		if(syminfo->name[syminfo->namelen - 1] == '\n')
			syminfo->name[--syminfo->namelen] = '\0';

		_(close)(fd);
		long got = _(readlink)("/proc/self/exe", localbuf, sizeof(syminfo->name));

		if(got > 0 && _(strncmp)(syminfo->name, localbuf, (ulong) got)) // Not the executable, therefore a dynamic library.
			syminfo->adjptr = ((char*) syminfo->adjptr) - ((ulong) syminfo->begin); // Remove the base loading address to obtain the relative address of the code.

		return syminfo;
	}
}

static void _(pstacktrace_with_context)(int fd, unw_context_t* ctx)
{
	long long calls = 1;
	unw_word_t previp = 0;
	unw_cursor_t cursor;

	if(SAFE(LIBUNWIND_LOCAL) unw_init_local(&cursor, ctx) != 0)
		return;

	do
	{
		unw_word_t offset;
		unw_word_t ip;
		char pname[1024]; // C++ says compilers should support at least this identifier length. I say that any more is overkill.
		SAFE(LIBUNWIND_LOCAL) unw_get_reg(&cursor, UNW_REG_IP, &ip);

		if(ip == previp)
		{
			++calls;
			continue;
		}

		if(calls > 1)
		{
			_(fprintf)(fd, " * %lld", calls); // For recursive functions print a single line with the number of consecutive calls.
			calls = 1;
		}

		if(previp)
			_(fprintf)(fd, "\n");

		previp = ip;
		SAFE(LIBUNWIND_LOCAL) unw_get_proc_name(&cursor, pname, sizeof(pname), &offset);

		if(ip == 0)
			break;

		aslibsyminfo syminfo;
		_(strcpy)(syminfo.name, "?");
		_(getsyminfo)((void*) ((uintptr_t) ip), &syminfo);
		_(fprintf)(fd, "    %s+%P (%s+%P)", *pname ? pname : "?", offset, syminfo.name, syminfo.adjptr);
	} while(SAFE(LIBUNWIND_LOCAL) unw_step(&cursor) > 0);

	if(calls > 1)
		_(fprintf)(fd, " * %lld", calls);

	if(previp)
		_(fprintf)(fd, "\n");
}

static void _(pmtstacktrace_handler)(assigctx* ctx)
{
	_(pthreadinfo)(tracefd);
	_(fputs)(":\n", tracefd);
	_(pstacktrace)(tracefd, ctx);
	++tids;
}

#ifdef __arm__
	#define convert_context(extra) ((unw_context_t*) ((void*) (&((ucontext_t*) (extra))->arm_r0)))
#else
	#define convert_context(extra) ((unw_context_t*) (extra))
#endif

/* Public API */

void _(pstacktrace)(int fd, assigctx* ctx)
{
	if(ctx == NULL)
	{
		unw_context_t context;
		SAFE(LIBUNWIND_LOCAL) unw_getcontext(&context);
		_(pstacktrace_with_context)(fd, &context);
	}
	else
		_(pstacktrace_with_context)(fd, convert_context(ctx->extra));
}

void _(pmtstacktrace)(int fd, assigctx* ctx)
{
	long tid = _(gettid)();
	int signum = SIGSEGV;

	if(ctx)
		signum = ((siginfo_t*) ctx->info)->si_signo;

	tracefd = fd;
	tids = _(gettids)(_(getpid)());
	assighandler curhandler = _(setsighandler)(signum, &_(pmtstacktrace_handler));

	while(*tids != 0)
	{
		long current = *tids;

		if(tid == current)
			_(pmtstacktrace_handler)(ctx);
		else if(_(tkill)(current, signum) == 0)
			while(current == *tids);
		else
			++tids;
	}

	_(setsighandler)(signum, curhandler);
}
