#define ASUTILS_PRIVATE_DEF
#include <as/char.h>
#include <as/math.h>
#include <as/memory.h>
#include <as/string.h>

/* Public API */

char* _(dbltostr)(char* dst, double value, ulong padto, ulong decimals, char padchar)
{
	llong ival = (llong) value;
	value = (value - ival) * 10;

	if(value < 0.0)
		value = -value;

	if(padto > decimals + 1)
		padto -= decimals + 1;
	else
		padto = 0;

	char* ptr = _(lltostr)(dst, (ullong) ival, 10, 0, 1, "", padto, padchar);
	*ptr++ = '.';

	if(decimals != 0)
	{
		ulong i;

		for(i = 0 ; i < decimals - 1 ; ++i)
		{
			ullong uval = (ullong) value;
			*ptr++ = '0' + (char) uval;
			value = (value - uval) * 10;
		}

		*ptr++ = '0' + (char) _(ceil)(value);
	}

	*ptr = '\0';
	return ptr;
}

char* _(lltostr)(char* dst, ullong value, ullong radix, int uppercase, int issigned, const char* prefix, ulong padto, char padchar)
{
	if(radix < 2 || radix > 36)
		return NULL;

	int negative = 0;
	ulong pfxlen = _(strlen)(prefix);

	if (issigned && ((llong) value) < 0)
	{
		if(padchar != ' ')
		{
			*dst++ = '-';

			if(padto != 0)
				--padto;
		}
		else
			negative = 1;

		value = (ullong) -((llong) value);
	}

	if(padchar != ' ' && pfxlen != 0)
	{
		dst = _(strencpy)(dst, prefix, pfxlen + 1);

		if(padto > pfxlen)
			padto -= pfxlen;
		else
			padto = 0;
	}

	char a_10 = (uppercase ? 'A' : 'a') - 10;
	char* bufbegin;
	char* bufend;
	bufend = bufbegin = &dst[sizeof(llong) * 8 - 1];

	do
	{
		ullong newval = value / radix;
		ullong remainder = value - newval * radix;

		if(remainder <= 9)
			*bufbegin = '0' + (char) remainder;
		else
			*bufbegin = a_10 + (char) remainder;

		value = newval;
	} while (value != 0 && --bufbegin);

	if(padchar == ' ' && pfxlen != 0)
	{
		bufbegin -= pfxlen;
		_(memcpy)(bufbegin, prefix, pfxlen);
	}

	if(negative)
		*--bufbegin = '-';

	ulong len = (ulong) (bufend - bufbegin + 1);

	if(padto > len)
	{
		ulong padding = padto - len;
		len = padto;
		bufbegin -= padding;
		_(memset)(bufbegin, padchar, padding);
	}

	_(memcpy)(dst, bufbegin, len);
	dst[len] = '\0';
	return &dst[len];
}

int _(snprintf)(char* str, ulong len, const char* fmt, ...)
{
	_TO_VA(ap, fmt,
		int res = _(vsnprintf)(str, len, fmt, ap);
	);
	return res;
}

int _(sprintf)(char* str, const char* fmt, ...)
{
	_TO_VA(ap, fmt,
		int res = _(vsnprintf)(str, _UMAX(ulong), fmt, ap);
	);
	return res;
}

char* _(stpcpy)(char* dst, const char* src)
{
	return _(stpncpy)(dst, src, _UMAX(ulong));
}

char* _(stpncpy)(char* dst, const char* src, ulong n)
{
	while(n-- && (*dst = *src++))
		++dst;

	return dst;
}

const char* _(strchr)(const char* str, int c)
{
	while(*str && *str != c)
		++str;

	if(!*str)
		return NULL;

	return str;
}

int _(strcmp)(const char* s1, const char* s2)
{
	return _(strncmp)(s1, s2, _UMAX(ulong));
}

char* _(strcpy)(char* dst, const char* src)
{
	return _(strncpy)(dst, src, _UMAX(ulong));
}

char* _(strencpy)(char* dst, const char* src, ulong n)
{
	dst = _(stpncpy)(dst, src, n);
	*dst = '\0';
	return dst;
}

ulong _(strlen)(const char* s)
{
	const char* p = s;

	while(*p++);

	return (ulong) (p - s - 1);
}

char* _(strncpy)(char* dst, const char* src, ulong n)
{
	_(stpncpy)(dst, src, n);
	return dst;
}

int _(strncmp)(const char* s1, const char* s2, ulong n)
{
	while(n && (*s1 == *s2))
	{
		--n;
		++s1;
		++s2;
	}

	if(!n)
		return 0;

	return *s1 - *s2;
}

const char* _(strrchr)(const char* str, int c)
{
	const char* strp = str;

	while(*strp && *strp != c)
		++strp;

	while(strp != str && *strp != c)
		--strp;

	if(strp == str)
		return NULL;

	return strp;
}

llong _(strtoll)(const char* str, llong radix)
{
	if(radix < 2 || radix > 36)
		return NULL;

	llong l = 0;
	llong multiplier = 1;

	if(*str == '-')
		multiplier = -1;

	if(*str == '-' || *str == '+')
		++str;

	while(_(isalnum)(*str))
	{
		llong c = _(ctoi)(*str);

		if(c >= radix)
			break;

		l = l * radix + c;
		++str;
	}

	return l * multiplier;
}

int _(vsnprintf)(char* str, ulong len, const char* fmt, va_list ap)
{
	char buf[200];
	char* at = str;
	ulong used = 0;
	char next;

	while((next = *fmt++))
	{
		used = ((ulong) (at - str)) + 1;

		if(used >= len)
			break;

		if(next != '%')
		{
			*at++ = next;
			continue;
		}

		ulong remaining = len - used;
		char padchar = ' ';
		ulong padto = 0;
		ulong decimals = 6;
		const char* copyfrom = fmt - 1;
		next = *fmt++;

		if(next == '0')
		{
			padchar = '0';
			next = *fmt++;
		}

		while(_(isdigit)(next))
		{
			padto *= 10;
			padto += (ulong) _(ctoi)(next);
			next = *fmt++;
		}

		if(next == '\0')
			break;

		if(next == '.')
		{
			next = *fmt++;

			if(_(isdigit)(next))
			{
				decimals = 0;

				do
				{
					decimals *= 10;
					decimals += (ulong) _(ctoi)(next);
					next = *fmt++;
				} while(_(isdigit)(next));
			}
		}

		ulong radix = 16;
		ulong slen;
		ullong u;
		const char* prefix = "";
		int size = 1;

		for(u = 0 ; u < 2 ; ++u)
		{
			if(next == 'l')
			{
				++size;
				next = *fmt++;
			}
		}

		if(next == '\0')
			break;

		switch(next)
		{
			case 'b':
				radix = 2;
			case 'o':
				if(radix == 16)
					radix = 8;
			case 'd':
			case 'i':
			case 'u':
				if(radix == 16)
					radix = 10;
			case 'P':
			case 'p':
				if(next == 'p' || next == 'P')
					prefix = "0x";
			case 'x':
			case 'X':
				if(size == 1)
					u = (ullong) va_arg(ap, uint);
				else if(size == 2)
					u = (ullong) va_arg(ap, ulong);
				else
					u = va_arg(ap, ullong);
				break;

			case 's':
				copyfrom = va_arg(ap, char*);
				break;

			case 'c' :
				copyfrom = buf;
				buf[0] = (char) va_arg(ap, int);
				buf[1] = '\0';
				break;
		}

		if(next == 'p' && u == 0)
		{
			copyfrom = "(nil)";
			next = 's';
		}

		switch (next)
		{
			case 'b':
			case 'o':
			case 'd':
			case 'i':
			case 'u':
			case 'p':
			case 'P':
			case 'x':
			case 'X':
				copyfrom = buf;
				_(lltostr)(buf, u, radix, (next == 'P' || next == 'X'), (next == 'd' || next == 'i'), prefix, padto, padchar);
				break;

			case 'f':
			case 'g':
				copyfrom = buf;
				_(dbltostr)(buf, va_arg(ap, double), padto, decimals, padchar);
				break;

			case 's':
			case 'c' :
				slen = _(strlen)(copyfrom);

				if(padto > slen)
				{
					ulong padding = padto - slen;

					if(padding > remaining)
						padding = remaining;

					remaining -= padding;
					at = _(lltostr)(at, 0, 10, 0, 0, "", padding + 1, ' ') - 1;

					if(remaining == 0)
						continue;
				}
		}

		char* newat = _(strencpy)(at, copyfrom, remaining);
		at = newat + (newat != at + _(strlen)(copyfrom));
	}

	*at = '\0';
	return (int) (at - str);
}

int _(vsprintf)(char* str, const char* fmt, va_list ap)
{
	return _(vsnprintf)(str, _UMAX(ulong), fmt, ap);
}
