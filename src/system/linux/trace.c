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
static volatile ulong index;

static aslibsyminfo* as_getsyminfo(void* ptr, aslibsyminfo* syminfo)
{
	char localbuf[512];
	syminfo->adjptr = ptr;

	// strlen("/proc/") + sizeof(long) * max_decimals_per_byte + strlen("/task/") + sizeof(long) * max_decimals_per_byte + strlen("/maps") + '\0'.
	char mapdir[6 + sizeof(long) * 3 + 6 + sizeof(long) * 3 + 5 + 1];
	as_sprintf(mapdir, "/proc/%ld/task/%ld/maps", as_getpid(), as_gettid());

	int fd = as_fopen(mapdir, "r");

	if(fd == -1)
	{
		as_sprintf(mapdir, "/proc/%ld/maps", as_gettid());
		fd = as_fopen(mapdir, "r");
	}

	if(fd == -1)
		return NULL;

	while(1)
	{
		char* line = as_fgets(localbuf, sizeof(localbuf), fd);

		if(!line)
		{
			as_close(fd);
			return NULL;
		}

		syminfo->begin = (void*) ((ulong) as_strtoll(line, 16));

		if(ptr < syminfo->begin)
			continue;

		const char* at = as_strchr(line, '-');

		if(!at)
			continue;

		syminfo->end = (void*) ((ulong) as_strtoll(++at, 16));

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

		syminfo->namelen = (ushort) (as_stpncpy(syminfo->name, at, sizeof(syminfo->name)) - syminfo->name);

		if(syminfo->name[syminfo->namelen - 1] == '\n')
			syminfo->name[--syminfo->namelen] = '\0';

		as_close(fd);
		long got = as_readlink("/proc/self/exe", localbuf, sizeof(syminfo->name));

		if(got > 0 && as_strncmp(syminfo->name, localbuf, (ulong) got)) // Not the executable, therefore a dynamic library.
			syminfo->adjptr = ((char*) syminfo->adjptr) - ((ulong) syminfo->begin); // Remove the base loading address to obtain the relative address of the code.

		return syminfo;
	}
}

static void as_pstacktrace_with_context(int fd, unw_context_t* ctx)
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
			as_fprintf(fd, " * %lld", calls); // For recursive functions print a single line with the number of consecutive calls.
			calls = 1;
		}

		if(previp)
			as_fprintf(fd, "\n");

		previp = ip;
		SAFE(LIBUNWIND_LOCAL) unw_get_proc_name(&cursor, pname, sizeof(pname), &offset);

		if(ip == 0)
			break;

		aslibsyminfo syminfo;
		as_strcpy(syminfo.name, "?");
		as_getsyminfo((void*) ((uintptr_t) ip), &syminfo);
		as_fprintf(fd, "    %s+%P (%s+%P)", *pname ? pname : "?", offset, syminfo.name, syminfo.adjptr);
	} while(SAFE(LIBUNWIND_LOCAL) unw_step(&cursor) > 0);

	if(calls > 1)
		as_fprintf(fd, " * %lld", calls);

	if(previp)
		as_fprintf(fd, "\n");
}

static void as_pmtstacktrace_handler(as_sigctx_t* ctx)
{
	as_pthreadinfo(tracefd);
	as_fputs(":\n", tracefd);
	as_pstacktrace(tracefd, ctx);
	++index;
}

#ifdef __arm__
	#define convert_context(extra) ((unw_context_t*) ((void*) (&((ucontext_t*) (extra))->arm_r0)))
#else
	#define convert_context(extra) ((unw_context_t*) (extra))
#endif

/* Public API */

void as_pstacktrace(int fd, as_sigctx_t* ctx)
{
	if(ctx == NULL)
	{
		unw_context_t context;
		SAFE(LIBUNWIND_LOCAL) unw_getcontext(&context);
		as_pstacktrace_with_context(fd, &context);
	}
	else
		as_pstacktrace_with_context(fd, convert_context(ctx->extra));
}

void as_pmtstacktrace(int fd, as_sigctx_t* ctx)
{
	long tid = as_gettid();
	int signum = SIGSEGV;

	if(ctx)
		signum = ((siginfo_t*) ctx->info)->si_signo;

	index = 0;
	tracefd = fd;
	long fewtids[64];
	long* tids = fewtids;
	ulong count = as_gettids(tids, 64);

	if(count > 64)
	{
		tids = (long*) as_malloc(sizeof(long) * count);
		ulong newcount = as_gettids(tids, count);

		if(newcount < count)
			count = newcount;
	}

	as_sighandler_t curhandler = as_setsighandler(signum, &as_pmtstacktrace_handler);

	while(index < count)
	{
		ulong current = index;
		long cur_tid = tids[index];

		if(tid == cur_tid)
			as_pmtstacktrace_handler(ctx);
		else if(as_tkill(cur_tid, signum) == 0)
			while(current == index);
		else
			++tids;
	}

	if(tids != fewtids)
		as_free(tids);

	as_setsighandler(signum, curhandler);
}
