#ifndef AS_SIGNAL_H_INCLUDED_
#define AS_SIGNAL_H_INCLUDED_
#include <as/utils.h>

typedef void (*assighandler)(assigctx* ctx);

#define AS_SIG_DEFAULT ((assighandler)  0)
#define AS_SIG_IGNORE  ((assighandler) ~0)

ASUTILS_BEGIN_CXXCOMPAT

assighandler ASUTILS_FUNC(setsighandler)(int signum, assighandler handler);

ASUTILS_END_CXXCOMPAT

#endif
