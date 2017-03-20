#include <dlfcn.h>

#include <libunwind.h>

#define ASUTILS_PRIVATE_DEF
#include <as/trace.h>
#include <as/fdio.h>
#include <as/utils.h>
#include <as/string.h>

/* Private */

static void _(pstacktrace_with_context)(int fd, unw_context_t* ctx)
{
	long long calls = 1;
	unw_word_t previp = 0;
	Dl_info dlinfo;
	unw_cursor_t cursor;
	
	if(unw_init_local(&cursor, ctx) == 0) do
	{
		unw_word_t offset;
		unw_word_t ip = 0;
		char pname[1024]; // C++ says compilers should support at least this identifier length. I say that any more is overkill.
		unw_get_reg(&cursor, UNW_REG_IP, &ip);
		
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
		
		unw_get_proc_name(&cursor, pname, sizeof(pname), &offset);

		if(ip == 0)
			break;

		/* TODO: create AS-Safe replacements for dladdr and dlopen using /proc/self/maps. */
		
		dladdr((void*) ((uintptr_t) ip), &dlinfo);

		if(dlopen(dlinfo.dli_fname, RTLD_LAZY) != NULL)
			ip -= (uintptr_t) dlinfo.dli_fbase; // For PIC remove the base loading address to obtain the relative address of the code.

		_(fprintf)(fd, "    %s+%P (%s+%P)", *pname ? pname : "?", offset, dlinfo.dli_fname, ip);
	} while (unw_step(&cursor) > 0);
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
		unw_getcontext(&context);
		_(pstacktrace_with_context)(fd, &context);
	}
	else
		_(pstacktrace_with_context)(fd, convert_context(ctx->extra));
}
