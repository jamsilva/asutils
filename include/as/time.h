#ifndef AS_TIME_H_INCLUDED_
#define AS_TIME_H_INCLUDED_
#include <as/utils.h>

typedef struct
{
	ulong year;
	utiny month;
	utiny day;
	utiny hour;
	utiny minute;
	utiny second;
} astime;

ASUTILS_BEGIN_CXXCOMPAT

astime ASUTILS_FUNC(totime)(ulong secs);

ASUTILS_END_CXXCOMPAT

#endif
