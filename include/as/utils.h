#ifndef AS_UTILS_H_INCLUDED_
#define AS_UTILS_H_INCLUDED_

typedef   signed char      tiny;
typedef unsigned char      utiny;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef   signed long long llong;
typedef unsigned long long ullong;

typedef struct
{
	ulong  inode;
	ulong  offset;
	ushort namelen;
	char   name[1];
} asdirent;

typedef struct
{
	void* info;
	void* extra;
} assigctx;

typedef struct
{
	ulong year;
	utiny month;
	utiny day;
	utiny hour;
	utiny minute;
	utiny second;
} astime;

#include <as/prefix_.h>

#ifdef __cplusplus
	#define ASUTILS_BEGIN_CXXCOMPAT extern "C" {
	#define ASUTILS_END_CXXCOMPAT   }
#else
	#define ASUTILS_BEGIN_CXXCOMPAT
	#define ASUTILS_END_CXXCOMPAT
#endif

#define ASUTILS_CAT0(a, b)      a ## b
#define ASUTILS_CAT(a, b)       ASUTILS_CAT0(a, b)
#define ASUTILS_FUNC(fname)     ASUTILS_CAT(ASUTILS_PREFIX, fname)

#include <as/system_.h>

#endif
