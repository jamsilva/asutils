#ifndef AS_UTILS_H_INCLUDED_
#define AS_UTILS_H_INCLUDED_

typedef   signed char      tiny;
typedef unsigned char      utiny;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef   signed long long llong;
typedef unsigned long long ullong;

#include <as/prefix_.h>

#ifdef __cplusplus
	#define ASUTILS_BEGIN_CXXCOMPAT extern "C" {
	#define ASUTILS_END_CXXCOMPAT   }
#else
	#define ASUTILS_BEGIN_CXXCOMPAT
	#define ASUTILS_END_CXXCOMPAT
#endif

#ifndef NULL
	#define NULL 0L
#endif

#define ASUTILS_ALIGN(v, align) ((ulong) (((v) + ((align) - 1)) & ~((align) - 1)))
#define ASUTILS_CAT0(a, b)      a ## b
#define ASUTILS_CAT(a, b)       ASUTILS_CAT0(a, b)
#define ASUTILS_FUNC(fname)     ASUTILS_CAT(ASUTILS_PREFIX, fname)
#define ASUTILS_SMAX(type)      (((type) 1) << (sizeof(type) * 8 - 1))
#define ASUTILS_UMAX(type)      (~((type) 0))

#define ASUTILS_TO_VA(ap, arg, ...) \
	va_list ap; \
	va_start(ap, arg); \
	__VA_ARGS__; \
	va_end(ap)

#ifdef ASUTILS_PRIVATE_DEF
	#define _ALIGN   ASUTILS_ALIGN
	#define _(fname) ASUTILS_FUNC(fname)
	#define _SMAX    ASUTILS_SMAX
	#define _UMAX    ASUTILS_UMAX
	#define _TO_VA   ASUTILS_TO_VA
#endif

#include <as/system.h>

#endif
