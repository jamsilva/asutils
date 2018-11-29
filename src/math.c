#include <as/private_.h>

/* Public API */

llong as_ceil(double d)
{
	if(d >= 0.0)
		return ((llong) d) + 1;

	return (llong) d;
}

llong as_floor(double d)
{
	if(d >= 0.0)
		return (llong) d;

	return ((llong) d) - 1;
}

llong as_max(llong l, llong r)
{
	return (l > r ? l : r);
}

llong as_min(llong l, llong r)
{
	return (l < r ? l : r);
}

llong as_round(double d)
{
	if(d >= 0.0)
		return (llong) (d + 0.5);

	return (llong) (d - 0.5);
}
