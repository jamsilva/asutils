#ifndef AS_SIGNAL_H_INCLUDED_
#define AS_SIGNAL_H_INCLUDED_
#include <as/utils.h>

typedef struct
{
	void* info;
	void* extra;
} assigctx;

typedef void (*assighandler)(assigctx* ctx);

ASUTILS_BEGIN_CXXCOMPAT

int ASUTILS_FUNC(setsighandler)(int signum, assighandler handler);

ASUTILS_END_CXXCOMPAT

#endif
