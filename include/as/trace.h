#ifndef AS_TRACE_H_INCLUDED_
#define AS_TRACE_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

void ASUTILS_FUNC(pstacktrace)  (int fd, assigctx* ctx);
void ASUTILS_FUNC(pmtstacktrace)(int fd, assigctx* ctx);

ASUTILS_END_CXXCOMPAT

#endif
