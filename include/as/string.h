#ifndef AS_STRING_H_INCLUDED_
#define AS_STRING_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

#include <stdarg.h>

char*       ASUTILS_FUNC(dbltostr) (char* dst, double value, ulong padto, ulong decimals, char padchar);
char*       ASUTILS_FUNC(lltostr)  (char* dst, ullong value, ullong radix, int uppercase, int issigned, const char* prefix, ulong padto, char padchar);
int         ASUTILS_FUNC(snprintf) (char* str, ulong len, const char* fmt, ...);
int         ASUTILS_FUNC(sprintf)  (char* str, const char* fmt, ...);
char*       ASUTILS_FUNC(stpcpy)   (char* dst, const char* src);
char*       ASUTILS_FUNC(stpncpy)  (char* dst, const char* src, ulong n);
const char* ASUTILS_FUNC(strchr)   (const char* str, int c);
int         ASUTILS_FUNC(strcmp)   (const char* s1, const char* s2);
char*       ASUTILS_FUNC(strcpy)   (char* dst, const char* src);
char*       ASUTILS_FUNC(strencpy) (char* dst, const char* src, ulong n);
ulong       ASUTILS_FUNC(strlen)   (const char* s);
char*       ASUTILS_FUNC(strncpy)  (char* dst, const char* src, ulong n);
int         ASUTILS_FUNC(strncmp)  (const char* s1, const char* s2, ulong n);
const char* ASUTILS_FUNC(strrchr)  (const char* str, int c);
llong       ASUTILS_FUNC(strtoll)  (const char* str, llong radix);
int         ASUTILS_FUNC(vsnprintf)(char* str, ulong len, const char* fmt, va_list va);
int         ASUTILS_FUNC(vsprintf) (char* str, const char* fmt, va_list va);

ASUTILS_END_CXXCOMPAT

#endif
