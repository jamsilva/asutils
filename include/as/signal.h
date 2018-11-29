#ifndef AS_SIGNAL_H_INCLUDED_
#define AS_SIGNAL_H_INCLUDED_
#include <as/utils.h>
#include <as/types/sigctx.h>

typedef void (*as_sighandler_t)(as_sigctx_t* ctx);

#define AS_SIG_DEFAULT ((as_sighandler_t)  0)
#define AS_SIG_IGNORE  ((as_sighandler_t) ~0)

ASUTILS_BEGIN_CXXCOMPAT

as_sighandler_t as_setsighandler(int signum, as_sighandler_t handler);

ASUTILS_END_CXXCOMPAT

#endif
