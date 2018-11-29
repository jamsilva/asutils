#ifndef AS_MATH_H_INCLUDED_
#define AS_MATH_H_INCLUDED_
#include <as/utils.h>

ASUTILS_BEGIN_CXXCOMPAT

llong as_ceil (double d);
llong as_floor(double d);
llong as_max  (llong l, llong r);
llong as_min  (llong l, llong r);
llong as_round(double d);

ASUTILS_END_CXXCOMPAT

#endif
