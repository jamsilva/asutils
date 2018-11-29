#ifndef AS_TASK_H_INCLUDED_
#define AS_TASK_H_INCLUDED_
#include <as/utils.h>
#include <as/types/sigctx.h>
#include <as/types/time.h>

ASUTILS_BEGIN_CXXCOMPAT

void as_psiginfo   (int fd, as_sigctx_t* ctx);
void as_pthreadinfo(int fd);
void as_ptimeinfo  (int fd, as_time_t* time);

ASUTILS_END_CXXCOMPAT

#endif
