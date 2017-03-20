#ifndef AS_MEMORY_H_INCLUDED_
#define AS_MEMORY_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

void* ASUTILS_FUNC(memcpy) (void* dst, const void* src, ulong len);
void* ASUTILS_FUNC(memset) (void* s, int c, ulong len);
void* ASUTILS_FUNC(memmove)(void* dst, const void* src, ulong len);

ASUTILS_END_CXXCOMPAT

#endif
