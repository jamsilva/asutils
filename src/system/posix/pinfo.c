#include <as/private_.h>

#ifndef _NSIG
	#define _NSIG 65
#endif

/* Public API */

void as_psiginfo(int fd, as_sigctx_t* ctx)
{
	siginfo_t* info = (siginfo_t*) ctx->info;
	as_pthreadinfo(fd);
	as_fputs(" received ", fd);

	#define PRINT_IF_SIG_EXTRA(begin, end, name, msg, ...) \
		if(info->si_signo >= begin && info->si_signo <= end) \
			as_fprintf(fd, name " (%d) - " msg, __VA_ARGS__); \
		else

	#define PRINT_IF_SIG(sig, msg) \
		PRINT_IF_SIG_EXTRA(sig, sig, #sig, msg, info->si_signo)

	#define ELSE

#ifdef SIGABRT
	PRINT_IF_SIG      (SIGABRT,                                     "Abort signal from abort()")
#endif
#ifdef SIGALRM
	PRINT_IF_SIG      (SIGALRM,                                     "Timer signal from alarm()")
#endif
#ifdef SIGBUS
	PRINT_IF_SIG      (SIGBUS,                                      "Bus error (bad memory access)")
#endif
#ifdef SIGCHLD
	PRINT_IF_SIG      (SIGCHLD,                                     "Child stopped or terminated")
#endif
#ifdef SIGCONT
	PRINT_IF_SIG      (SIGCONT,                                     "Continue if stopped")
#endif
#ifdef SIGEMT
	PRINT_IF_SIG      (SIGEMT,                                      "Emulator trap")
#endif
#ifdef SIGFPE
	PRINT_IF_SIG      (SIGFPE,                                      "Arithmetic exception")
#endif
#ifdef SIGHUP
	PRINT_IF_SIG      (SIGHUP,                                      "Hangup detected on controlling terminal or death of controlling process (Ctrl + D)")
#endif
#ifdef SIGILL
	PRINT_IF_SIG      (SIGILL,                                      "Illegal instruction")
#endif
#ifdef SIGINT
	PRINT_IF_SIG      (SIGINT,                                      "Interrupt from keyboard (Ctrl + C)")
#endif
#ifdef SIGIO
	PRINT_IF_SIG      (SIGIO,                                       "IO now possible")
#endif
#ifdef SIGKILL
	PRINT_IF_SIG      (SIGKILL,                                     "Kill signal")
#endif
#ifdef SIGPIPE
	PRINT_IF_SIG      (SIGPIPE,                                     "Broken pipe: write to pipe with no readers")
#endif
#ifdef SIGPROF
	PRINT_IF_SIG      (SIGPROF,                                     "Profiling timer expired")
#endif
#ifdef SIGPWR
	PRINT_IF_SIG      (SIGPWR,                                      "Power failure")
#endif
#ifdef SIGQUIT
	PRINT_IF_SIG      (SIGQUIT,                                     "Quit from keyboard (Ctrl + \\)")
#endif
#ifdef SIGSEGV
	PRINT_IF_SIG      (SIGSEGV,                                     "Invalid memory reference")
#endif
#ifdef SIGSTKFLT
	PRINT_IF_SIG      (SIGSTKFLT,                                   "Stack fault on coprocessor")
#endif
#ifdef SIGSTOP
	PRINT_IF_SIG      (SIGSTOP,                                     "Stop process")
#endif
#ifdef SIGSYS
	PRINT_IF_SIG      (SIGSYS,                                      "Bad argument to routine")
#endif
#ifdef SIGTERM
	PRINT_IF_SIG      (SIGTERM,                                     "Termination signal")
#endif
#ifdef SIGTRAP
	PRINT_IF_SIG      (SIGTRAP,                                     "Trace/breakpoint trap")
#endif
#ifdef SIGTSTP
	PRINT_IF_SIG      (SIGTSTP,                                     "Stop typed at terminal (Ctrl + Z)")
#endif
#ifdef SIGTTIN
	PRINT_IF_SIG      (SIGTTIN,                                     "Terminal input for background process")
#endif
#ifdef SIGTTOU
	PRINT_IF_SIG      (SIGTTOU,                                     "Terminal output for background process")
#endif
#ifdef SIGUSR1
	PRINT_IF_SIG      (SIGUSR1,                                     "User-defined signal 1")
#endif
#ifdef SIGUSR2
	PRINT_IF_SIG      (SIGUSR2,                                     "User-defined signal 2")
#endif
#ifdef SIGURG
	PRINT_IF_SIG      (SIGURG,                                      "Urgent condition on socket")
#endif
#ifdef SIGVTALRM
	PRINT_IF_SIG      (SIGVTALRM,                                   "Virtual alarm clock")
#endif
#ifdef SIGWINCH
	PRINT_IF_SIG      (SIGWINCH,                                    "Window resize signal")
#endif
#ifdef SIGXCPU
	PRINT_IF_SIG      (SIGXCPU,                                     "CPU time limit exceeded")
#endif
#ifdef SIGXFSZ
	PRINT_IF_SIG      (SIGXFSZ,                                     "File size limit exceeded")
#endif
#ifdef SIGRTMIN
	PRINT_IF_SIG      (SIGRTMIN,                                    "Real-time signal 1")
#endif
#ifdef SIGRTMAX
	PRINT_IF_SIG_EXTRA(SIGRTMAX,     SIGRTMAX,     "SIGRTMAX",      "Real-time signal %d",                            info->si_signo, info->si_signo - SIGRTMIN + 1)
#endif
#if defined(SIGRTMIN) && defined(SIGRTMAX)
	PRINT_IF_SIG_EXTRA(SIGRTMIN + 1, SIGRTMAX - 1, "SIGRTMIN + %d", "Real-time signal %d", info->si_signo - SIGRTMIN, info->si_signo, info->si_signo - SIGRTMIN + 1)
#endif
	ELSE as_fprintf(fd,                                             "Unknown signal (%d)",                            info->si_signo);

	as_fputs(":\n    ", fd);

	const char* extra_message;
	int print_address = 1;
	int reason_was_printed = 1;

	#define PRINT_IF_CODE_EMSG(code, msg, msgm) \
		if(info->si_code == code) \
		{ \
			as_fprintf(fd, #code " (%d) - " msg, code); \
			extra_message = msgm; \
		} \
		else

	#define PRINT_IF_CODE(code, msg) \
		PRINT_IF_CODE_EMSG(code, msg, "")

	#define PRINT_IF_CODE_AT(code, msg) \
		PRINT_IF_CODE_EMSG(code, msg, " at")

	#define IF_NO_CODE reason_was_printed = 0;

#ifdef SIGBUS
	if(info->si_signo == SIGBUS)
	{
	#ifdef BUS_ADRALN
		PRINT_IF_CODE_AT(BUS_ADRALN,    "Invalid alignment")
	#endif
	#ifdef BUS_ADRERR
		PRINT_IF_CODE   (BUS_ADRERR,    "Nonexistent physical")
	#endif
	#ifdef BUS_OBJERR
		PRINT_IF_CODE_AT(BUS_OBJERR,    "Object-specific hardware error")
	#endif
	#ifdef BUS_MCEERR_AR
		PRINT_IF_CODE_AT(BUS_MCEERR_AR, "Hardware memory error consumed on a machine check; action required")
	#endif
	#ifdef BUS_MCEERR_AO
		PRINT_IF_CODE_AT(BUS_MCEERR_AO, "Hardware memory error detected in process but not consumed; action optional")
	#endif
		IF_NO_CODE
	}
	else
#endif
#ifdef SIGCHLD
	if(info->si_signo == SIGCHLD)
	{
		print_address = 0;

	#ifdef CLD_DUMPED
		PRINT_IF_CODE_EMSG(CLD_DUMPED,    "C",         "terminated abnormally")
	#endif
	#ifdef CLD_EXITED
		PRINT_IF_CODE_EMSG(CLD_EXITED,    "C",         "exited")
	#endif
	#ifdef CLD_KILLED
		PRINT_IF_CODE_EMSG(CLD_KILLED,    "C",         "was killed")
	#endif
	#ifdef CLD_STOPPED
		PRINT_IF_CODE_EMSG(CLD_STOPPED,   "C",         "has stopped")
	#endif
	#ifdef CLD_TRAPPED
		PRINT_IF_CODE_EMSG(CLD_TRAPPED,   "Traced c",  "has trapped")
	#endif
	#ifdef CLD_CONTINUED
		PRINT_IF_CODE_EMSG(CLD_CONTINUED, "Stopped c", "has continued")
	#endif
		IF_NO_CODE

		if(reason_was_printed)
		{
			as_fprintf(fd, "hild with PID %d %s", info->si_pid, extra_message);

		#ifdef CLD_EXITED
			if(info->si_code == CLD_EXITED)
				as_fprintf(fd, " with status code %d", info->si_status);
		#endif
		}
	}
	else
#endif
#ifdef SIGFPE
	if(info->si_signo == SIGFPE)
	{
	#ifdef FPE_FLTDIV
		PRINT_IF_CODE_AT(FPE_FLTDIV, "Floating-point divide by zero")
	#endif
	#ifdef FPE_FLTINV
		PRINT_IF_CODE_AT(FPE_FLTINV, "Floating-point invalid operation")
	#endif
	#ifdef FPE_FLTOVF
		PRINT_IF_CODE_AT(FPE_FLTOVF, "Floating-point overflow")
	#endif
	#ifdef FPE_FLTRES
		PRINT_IF_CODE_AT(FPE_FLTRES, "Floating-point inexact result")
	#endif
	#ifdef FPE_FLTSUB
		PRINT_IF_CODE_AT(FPE_FLTSUB, "Subscript out of range")
	#endif
	#ifdef FPE_FLTUND
		PRINT_IF_CODE_AT(FPE_FLTUND, "Floating-point underflow")
	#endif
	#ifdef FPE_INTDIV
		PRINT_IF_CODE_AT(FPE_INTDIV, "Integer divide by zero")
	#endif
	#ifdef FPE_INTOVF
		PRINT_IF_CODE_AT(FPE_INTOVF, "Integer overflow")
	#endif
		IF_NO_CODE
	}
	else
#endif
#ifdef SIGILL
	if(info->si_signo == SIGILL)
	{
	#ifdef ILL_BADSTK
		PRINT_IF_CODE_AT(ILL_BADSTK, "Internal stack error")
	#endif
	#ifdef ILL_COPROC
		PRINT_IF_CODE_AT(ILL_COPROC, "Coprocessor error")
	#endif
	#ifdef ILL_ILLADR
		PRINT_IF_CODE_AT(ILL_ILLADR, "Illegal addressing mode")
	#endif
	#ifdef ILL_ILLOPC
		PRINT_IF_CODE_AT(ILL_ILLOPC, "Illegal opcode")
	#endif
	#ifdef ILL_ILLOPN
		PRINT_IF_CODE_AT(ILL_ILLOPN, "Illegal operand")
	#endif
	#ifdef ILL_ILLTRP
		PRINT_IF_CODE_AT(ILL_ILLTRP, "Illegal trap")
	#endif
	#ifdef ILL_PRVOPC
		PRINT_IF_CODE_AT(ILL_PRVOPC, "Privileged opcode")
	#endif
	#ifdef ILL_PRVREG
		PRINT_IF_CODE_AT(ILL_PRVREG, "Privileged register")
	#endif
		IF_NO_CODE
	}
	else
#endif
#ifdef SIGIO
	if(info->si_signo == SIGIO)
	{
		print_address = 0;

	#ifdef POLL_ERR
		PRINT_IF_CODE(POLL_ERR, "IO error")
	#endif
	#ifdef POLL_HUP
		PRINT_IF_CODE(POLL_HUP, "Device disconnected")
	#endif
	#ifdef POLL_IN
		PRINT_IF_CODE(POLL_IN,  "Data input available")
	#endif
	#ifdef POLL_MSG
		PRINT_IF_CODE(POLL_MSG, "Input message available")
	#endif
	#ifdef POLL_OUT
		PRINT_IF_CODE(POLL_OUT, "Output buffers available")
	#endif
	#ifdef POLL_PRI
		PRINT_IF_CODE(POLL_PRI, "High priority input available")
	#endif
		IF_NO_CODE

		if(reason_was_printed)
			as_fprintf(fd, "at file descriptor %d", info->si_fd);
	}
	else
#endif
#ifdef SIGSEGV
	if(info->si_signo == SIGSEGV)
	{
	#ifdef SEGV_ACCERR
		PRINT_IF_CODE_AT(SEGV_ACCERR, "Invalid permissions for mapped object")
	#endif
	#ifdef SEGV_MAPERR
		PRINT_IF_CODE_AT(SEGV_MAPERR, "No object mapped")
	#endif
	#ifdef SEGV_BNDERR
		PRINT_IF_CODE_AT(SEGV_BNDERR, "Failed bound checks")
	#endif
	#ifdef SEGV_PKUERR
		PRINT_IF_CODE_AT(SEGV_PKUERR, "Access was denied by memory protection keys")
	#endif
		IF_NO_CODE
	}
	else
#endif
#ifdef SIGTRAP
	if(info->si_signo == SIGTRAP)
	{
	#ifdef TRAP_BRKPT
		PRINT_IF_CODE_AT(TRAP_BRKPT,  "Process breakpoint")
	#endif
	#ifdef TRAP_TRACE
		PRINT_IF_CODE_AT(TRAP_TRACE,  "Process trace trap")
	#endif
	#ifdef TRAP_BRANCH
		PRINT_IF_CODE_AT(TRAP_BRANCH, "Process taken branch trap")
	#endif
	#ifdef TRAP_HWBKPT
		PRINT_IF_CODE_AT(TRAP_HWBKPT, "Hardware breakpoint")
	#endif
		IF_NO_CODE
	}
	else
#endif
#ifdef SIGSYS
	if(info->si_signo == SIGSYS)
	{
		print_address = 0;

	#ifdef SYS_SECCOMP
		PRINT_IF_CODE_AT(SYS_SECCOMP, "Triggered by a seccomp() filter rule")
	#endif
		IF_NO_CODE
	}
	else
#endif
		IF_NO_CODE

	if(!reason_was_printed)
	{
	#ifdef SI_ASYNCIO
		PRINT_IF_CODE(SI_ASYNCIO, "Asynchronous IO completed")
	#endif
	#ifdef SI_ASYNCNL
		PRINT_IF_CODE(SI_ASYNCNL, "Asynchronous name lookup completed")
	#endif
	#ifdef SI_KERNEL
		PRINT_IF_CODE(SI_KERNEL,  "Sent by the kernel")
	#endif
	#ifdef SI_QUEUE
		PRINT_IF_CODE(SI_QUEUE,   "Sent by sigqueue()")
	#endif
	#ifdef SI_USER
		PRINT_IF_CODE(SI_USER,    "Sent by kill()")
	#endif
	#ifdef SI_TIMER
		PRINT_IF_CODE(SI_TIMER,   "POSIX timer expired")
	#endif
	#ifdef SI_MESGQ
		PRINT_IF_CODE(SI_MESGQ,   "POSIX message queue state changed")
	#endif
	#ifdef SI_SIGIO
		PRINT_IF_CODE(SI_SIGIO,   "Queued SIGIO")
	#endif
	#ifdef SI_TKILL
		PRINT_IF_CODE(SI_TKILL,   "Sent by tkill() or tgkill()")
	#endif
		ELSE as_fprintf(fd,       "Unknown code %d", info->si_code);
	}
	else if(print_address)
		as_fprintf(fd, "%s address %p", extra_message, info->si_addr);

	as_fputs(".\n", fd);
}

void as_pthreadinfo(int fd)
{
	as_tname_t tname;
	as_gettname(tname);

	if(tname[0] != '\0')
		as_fprintf(fd, "Thread \"%s\"", tname);
	else
		as_fputs("Unnamed thread", fd);

	as_fprintf(fd, " (%d)", as_gettid());
}

void as_ptimeinfo(int fd, as_time_t* time)
{
	as_fprintf(fd, "%ld-%02d-%02d %02d:%02d:%02d UTC", time->year, time->month, time->day, time->hour, time->minute, time->second);
}
