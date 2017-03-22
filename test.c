#include <as/fdio.h>
#include <as/pinfo.h>
#include <as/string.h>
#include <as/signal.h>
#include <as/time.h>
#include <as/trace.h>

#include <pthread.h>
#include <signal.h>

void* loop(void* ignored)
{
	(void) ignored;
	as_settname("looper");

	while(1);

	return NULL;
}

static void handle_segv(assigctx* ctx)
{
	astime time = as_totime(as_timesecs());
	as_fputs("Crashed at ", AS_STDERR);
	as_ptimeinfo(AS_STDERR, &time);
	as_fputs("...\n\n", AS_STDERR);
	as_psiginfo(AS_STDERR, ctx);
	as_fputs("\nHere, look at the stack trace:\n\n", AS_STDERR);
	as_pmtstacktrace(AS_STDERR, ctx);
	as_pexit(42);
}

int main()
{
	pthread_t loop_th1, loop_th2;

	as_setsighandler(SIGSEGV, &handle_segv);

	pthread_create(&loop_th1, NULL, &loop, NULL);
	pthread_create(&loop_th2, NULL, &loop, NULL);

	as_pmtstacktrace(AS_STDERR, NULL);

	char* unmapped = (char*) NULL;
	*unmapped = '\0';

	pthread_join(loop_th1, NULL);
	pthread_join(loop_th2, NULL);
	return 0;
}
