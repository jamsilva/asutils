#ifndef AS_MEMORY_H_INCLUDED_
#define AS_MEMORY_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

void  as_free   (void* ptr);
void* as_malloc (ulong size);
void* as_memcpy (void* dst, const void* src, ulong len);
void* as_memset (void* s, int c, ulong len);
void* as_memmove(void* dst, const void* src, ulong len);

ASUTILS_END_CXXCOMPAT

#endif
