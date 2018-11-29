#ifndef AS_STRING_H_INCLUDED_
#define AS_STRING_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

#include <stdarg.h>

char*       as_dbltostr (char* dst, double value, ulong padto, ulong decimals, char padchar);
char*       as_lltostr  (char* dst, ullong value, ullong radix, int uppercase, int issigned, const char* prefix, ulong padto, char padchar);
int         as_snprintf (char* str, ulong len, const char* fmt, ...);
int         as_sprintf  (char* str, const char* fmt, ...);
char*       as_stpcpy   (char* dst, const char* src);
char*       as_stpncpy  (char* dst, const char* src, ulong n);
const char* as_strchr   (const char* str, int c);
int         as_strcmp   (const char* s1, const char* s2);
char*       as_strcpy   (char* dst, const char* src);
char*       as_strencpy (char* dst, const char* src, ulong n);
ulong       as_strlen   (const char* s);
char*       as_strncpy  (char* dst, const char* src, ulong n);
int         as_strncmp  (const char* s1, const char* s2, ulong n);
const char* as_strrchr  (const char* str, int c);
llong       as_strtoll  (const char* str, llong radix);
int         as_vsnprintf(char* str, ulong len, const char* fmt, va_list va);
int         as_vsprintf (char* str, const char* fmt, va_list va);

ASUTILS_END_CXXCOMPAT

#endif
