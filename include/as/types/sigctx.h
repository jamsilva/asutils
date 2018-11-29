#ifndef AS_TYPES_SIGCTX_H_INCLUDED_
#define AS_TYPES_SIGCTX_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

typedef struct as_sigctx_t
{
	void* info;
	void* extra;
} as_sigctx_t;

ASUTILS_END_CXXCOMPAT

#endif
