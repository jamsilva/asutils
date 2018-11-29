#include <as/fdio.h>
#include <as/pinfo.h>
#include <as/string.h>
#include <as/signal.h>
#include <as/time.h>
#include <as/trace.h>

#include <pthread.h>
#include <signal.h>

volatile int threads = 0;

static void put_separator(char c)
{
	as_fprintf(AS_STDERR, "%c---------------------------------------------------------------------%c\n", c, c);
}

static void handle_segv(as_sigctx_t* ctx)
{
	as_time_t time = as_totime(as_timesecs());
	put_separator(',');
	as_fputs("Crashed at ", AS_STDERR);
	as_ptimeinfo(AS_STDERR, &time);
	as_fputs("...\n\n", AS_STDERR);
	as_psiginfo(AS_STDERR, ctx);
	as_fputs("\nHere, look at the stack trace:\n\n", AS_STDERR);
	as_pmtstacktrace(AS_STDERR, ctx);
	put_separator('\'');
	as_pexit(42);
}

void* loop(void* ignored)
{
	(void) ignored;
	as_settname("looper");

	put_separator(',');
	as_pmtstacktrace(AS_STDERR, NULL);
	put_separator('\'');
	as_fputs("\n", AS_STDERR);
	++threads;

	while(1);

	return NULL;
}

int main()
{
	pthread_t loop_th1, loop_th2;

	as_setsighandler(SIGSEGV, &handle_segv);

	put_separator(',');
	as_pmtstacktrace(AS_STDERR, NULL);
	put_separator('\'');
	as_fputs("\n", AS_STDERR);

	pthread_create(&loop_th1, NULL, &loop, NULL);

	while(threads < 1);

	pthread_create(&loop_th2, NULL, &loop, NULL);

	while(threads < 2);

	// Without both volatiles, compilers may produce unexpected results...
	volatile char* volatile unmapped = NULL;
	*unmapped = '\0';

	pthread_join(loop_th1, NULL);
	pthread_join(loop_th2, NULL);
	return 0;
}
