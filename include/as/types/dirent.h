#ifndef AS_TYPES_DIRENT_H_INCLUDED_
#define AS_TYPES_DIRENT_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

#define AS_MAXDIRLEN 256

typedef struct as_dirent_t
{
	ulong  inode;
	ulong  offset;
	ushort namelen;
	char   name[AS_MAXDIRLEN];
} as_dirent_t;

ASUTILS_END_CXXCOMPAT

#endif
