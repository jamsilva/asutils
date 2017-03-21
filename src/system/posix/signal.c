/* System */
#include <signal.h>

#define ASUTILS_PRIVATE_DEF
#include <as/signal.h>
#include <as/utils.h>

/* Private */

static assighandler installed_handlers[_NSIG];

static void _(dispatcher)(int signum, siginfo_t* info, void* extra)
{
	assighandler handler = installed_handlers[signum];

	if(handler)
	{
		assigctx ctx;
		ctx.info = info;
		ctx.extra = extra;
		handler(&ctx);
	}
}

/* Public API */

int _(setsighandler)(int signum, assighandler handler)
{
	struct sigaction new_sa;
	installed_handlers[signum] = handler;
	new_sa.sa_sigaction = &_(dispatcher);
	sigfillset(&new_sa.sa_mask);
	new_sa.sa_flags = SA_ONSTACK | SA_RESTART | SA_SIGINFO;
	SAFE(POSIX.1-2001) return sigaction(signum, &new_sa, NULL);
}
