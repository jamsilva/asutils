#ifndef AS_TRACE_H_INCLUDED_
#define AS_TRACE_H_INCLUDED_
#include <as/utils.h>
#include <as/types/sigctx.h>

ASUTILS_BEGIN_CXXCOMPAT

void as_pstacktrace  (int fd, as_sigctx_t* ctx);
void as_pmtstacktrace(int fd, as_sigctx_t* ctx);

ASUTILS_END_CXXCOMPAT

#endif
