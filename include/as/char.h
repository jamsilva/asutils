#ifndef AS_CHAR_H_INCLUDED_
#define AS_CHAR_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

int as_ctoi    (int c);
int as_isalnum (int c);
int as_isalpha (int c);
int as_iscntrl (int c);
int as_isdigit (int c);
int as_isgraph (int c);
int as_islower (int c);
int as_isprint (int c);
int as_ispunct (int c);
int as_isspace (int c);
int as_isupper (int c);
int as_isxdigit(int c);
int as_isblank (int c);
int as_tolower (int c);
int as_toupper (int c);

ASUTILS_END_CXXCOMPAT

#endif
