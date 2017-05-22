/* System */
#include <signal.h>

#include <as/private_.h>

/* Private */

static assighandler installed_handlers[_NSIG] = {NULL};

static void _(dispatcher)(int signum, siginfo_t* info, void* extra)
{
	assighandler handler = installed_handlers[signum];

	if(handler != AS_SIG_DEFAULT && handler != AS_SIG_IGNORE)
	{
		assigctx ctx;
		ctx.info = info;
		ctx.extra = extra;
		handler(&ctx);
	}
}

/* Public API */

assighandler _(setsighandler)(int signum, assighandler handler)
{
	struct sigaction new_sa;
	assighandler curhandler = installed_handlers[signum];
	installed_handlers[signum] = handler;

	if(handler != AS_SIG_DEFAULT)
	{
		new_sa.sa_sigaction = &_(dispatcher);
		SAFE(POSIX.1-2001) sigfillset(&new_sa.sa_mask);
		new_sa.sa_flags = SA_ONSTACK | SA_RESTART | SA_SIGINFO;
		SAFE(POSIX.1-2001) sigaction(signum, &new_sa, NULL);
	}
	else
		SAFE(POSIX.1-2001) signal(signum, SIG_DFL);

	return curhandler;
}
