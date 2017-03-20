#ifndef AS_TASK_H_INCLUDED_
#define AS_TASK_H_INCLUDED_
#include <as/utils.h>
#include <as/time.h>
#include <as/signal.h>

ASUTILS_BEGIN_CXXCOMPAT

void ASUTILS_FUNC(psiginfo)   (int fd, assigctx* ctx);
void ASUTILS_FUNC(pthreadinfo)(int fd);
void ASUTILS_FUNC(ptimeinfo)  (int fd, astime* time);

ASUTILS_END_CXXCOMPAT

#endif
