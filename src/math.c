#include <as/private_.h>

/* Public API */

llong _(ceil)(double d)
{
	if(d >= 0.0)
		return ((llong) d) + 1;

	return (llong) d;
}

llong _(floor)(double d)
{
	if(d >= 0.0)
		return (llong) d;

	return ((llong) d) - 1;
}

llong _(max)(llong l, llong r)
{
	if(l > r)
		return l;

	return r;
}

llong _(min)(llong l, llong r)
{
	if(l < r)
		return l;

	return r;
}

llong _(round)(double d)
{
	if(d >= 0.0)
		return (llong) (d + 0.5);

	return (llong) (d - 0.5);
}
