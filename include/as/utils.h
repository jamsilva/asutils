#ifndef AS_UTILS_H_INCLUDED_
#define AS_UTILS_H_INCLUDED_

typedef   signed char      tiny;
typedef unsigned char      utiny;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef   signed long long llong;
typedef unsigned long long ullong;

#ifdef __cplusplus
	#define ASUTILS_BEGIN_CXXCOMPAT extern "C" {
	#define ASUTILS_END_CXXCOMPAT   }
#else
	#define ASUTILS_BEGIN_CXXCOMPAT
	#define ASUTILS_END_CXXCOMPAT
#endif

#include <as/system_.h>

#endif
