#ifndef AS_MATH_H_INCLUDED_
#define AS_MATH_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

llong ASUTILS_FUNC(ceil) (double d);
llong ASUTILS_FUNC(floor)(double d);
llong ASUTILS_FUNC(max)  (llong l, llong r);
llong ASUTILS_FUNC(min)  (llong l, llong r);
llong ASUTILS_FUNC(round)(double d);

ASUTILS_END_CXXCOMPAT

#endif
