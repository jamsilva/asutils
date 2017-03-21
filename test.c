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
	long i = 0;
	as_settname("looper");

	while(i++ < 1000)
		as_printf("%ld\n", i);

	char* unmapped = (char*) 0x42;
	*unmapped = '\0';

	return NULL;
}

static void handle_segv(assigctx* ctx)
{
	astime time = as_totime(as_timesecs());
	as_fputs("Crashed at ", AS_STDERR);
	as_ptimeinfo(AS_STDERR, &time);
	as_fputs(":\n", AS_STDERR);
	as_psiginfo(AS_STDERR, ctx);
	as_fputs("Here, look at the stack trace:\n", AS_STDERR);
	as_pstacktrace(AS_STDERR, ctx);
	as_fputs("\n", AS_STDERR);
	as_texit(1);
}

int main()
{
	pthread_t loop_th1, loop_th2;

	as_setsighandler(SIGSEGV, &handle_segv);

	pthread_create(&loop_th1, NULL, &loop, NULL);
	pthread_create(&loop_th2, NULL, &loop, NULL);
	as_fputs("\n", AS_STDERR);

	pthread_join(loop_th1, NULL);
	pthread_join(loop_th2, NULL);

	char* unmapped = (char*) NULL;
	*unmapped = '\0';
	return 0;
}
