#ifndef AS_CHAR_H_INCLUDED_
#define AS_CHAR_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

int ASUTILS_FUNC(ctoi)    (int c);
int ASUTILS_FUNC(isalnum) (int c);
int ASUTILS_FUNC(isalpha) (int c);
int ASUTILS_FUNC(iscntrl) (int c);
int ASUTILS_FUNC(isdigit) (int c);
int ASUTILS_FUNC(isgraph) (int c);
int ASUTILS_FUNC(islower) (int c);
int ASUTILS_FUNC(isprint) (int c);
int ASUTILS_FUNC(ispunct) (int c);
int ASUTILS_FUNC(isspace) (int c);
int ASUTILS_FUNC(isupper) (int c);
int ASUTILS_FUNC(isxdigit)(int c);
int ASUTILS_FUNC(isblank) (int c);
int ASUTILS_FUNC(tolower) (int c);
int ASUTILS_FUNC(toupper) (int c);

ASUTILS_END_CXXCOMPAT

#endif
