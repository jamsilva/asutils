#ifndef AS_TYPES_TIME_H_INCLUDED_
#define AS_TYPES_TIME_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

typedef struct as_time_t
{
	ulong year;
	utiny month;
	utiny day;
	utiny hour;
	utiny minute;
	utiny second;
} as_time_t;

ASUTILS_END_CXXCOMPAT

#endif
