#include <as/private_.h>

/* Private */

static as_sighandler_t installed_handlers[_NSIG] = { AS_SIG_DEFAULT };

static void as_dispatcher(int signum, siginfo_t* info, void* extra)
{
	as_sighandler_t handler = installed_handlers[signum];

	if(handler != AS_SIG_DEFAULT && handler != AS_SIG_IGNORE)
	{
		as_sigctx_t ctx;
		ctx.info = info;
		ctx.extra = extra;
		handler(&ctx);
	}
}

/* Public API */

as_sighandler_t as_setsighandler(int signum, as_sighandler_t handler)
{
	struct sigaction new_sa;
	as_sighandler_t curhandler = installed_handlers[signum];
	installed_handlers[signum] = handler;

	if(handler == AS_SIG_DEFAULT)
		SAFE(POSIX.1-2001) signal(signum, SIG_IGN);
	else if(handler == AS_SIG_IGNORE)
		SAFE(POSIX.1-2001) signal(signum, SIG_DFL);
	else
	{
		new_sa.sa_sigaction = &as_dispatcher;
		SAFE(POSIX.1-2001) sigfillset(&new_sa.sa_mask);
		new_sa.sa_flags = SA_ONSTACK | SA_RESTART | SA_SIGINFO;
		SAFE(POSIX.1-2001) sigaction(signum, &new_sa, NULL);
	}

	return curhandler;
}
