#include <dlfcn.h>

#include <libunwind.h>

#define ASUTILS_PRIVATE_DEF
#include <as/trace.h>
#include <as/fdio.h>
#include <as/utils.h>
#include <as/string.h>

/* Private */

typedef struct
{
	void*  begin;
	void*  end;
	void*  adjptr;
	ushort namelen;
	char   name[256];
} aslibsyminfo;

static aslibsyminfo* _(getsyminfo)(void* ptr, aslibsyminfo* syminfo)
{
	char localbuf[512];
	syminfo->adjptr = ptr;
	int fd = _(fopen)("/proc/self/maps", "r");

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

	if(SAFE(LIBUNWIND_LOCAL) unw_init_local(&cursor, ctx) == 0) do
	{
		unw_word_t offset;
		unw_word_t ip = 0;
		char pname[1024]; // C++ says compilers should support at least this identifier length. I say that any more is overkill.
		SAFE(LIBUNWIND_LOCAL) unw_get_reg(&cursor, UNW_REG_IP, &ip);

		if(ip == previp)
		{
			++calls;
			continue;
		}
		else
		{
			if(calls > 1)
			{
				_(fprintf)(fd, " * %lld", calls); // For recursive functions print a single line with the number of consecutive calls.
				calls = 1;
			}

			if(previp)
				_(fprintf)(fd, "\n");

			previp = ip;
		}

		SAFE(LIBUNWIND_LOCAL) unw_get_proc_name(&cursor, pname, sizeof(pname), &offset);

		if(ip == 0)
			break;

		aslibsyminfo syminfo;
		_(strcpy)(syminfo.name, "?");
		_(getsyminfo)((void*) ((uintptr_t) ip), &syminfo);
		_(fprintf)(fd, "    %s+%P (%s+%P)", *pname ? pname : "?", offset, syminfo.name, syminfo.adjptr);
	} while(SAFE(LIBUNWIND_LOCAL) unw_step(&cursor) > 0);
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
