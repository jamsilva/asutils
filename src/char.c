#include <as/private_.h>

/* Public API */

int as_ctoi(int c)
{
	c = as_tolower(c);

	if(as_islower(c))
		return c - 'a' + 10;

	if(as_isdigit(c))
		return c - '0';

	return -1;
}

int as_isalnum(int c)
{
	return as_isalpha(c) || as_isdigit(c);
}

int as_isalpha(int c)
{
	return as_isupper(c) || as_islower(c);
}

int as_iscntrl(int c)
{
	return c == 0x7F || (c >= '\0' && c <= 0x1F);
}

int as_isdigit(int c)
{
	return c >= '0' && c <= '9';
}

int as_isgraph(int c)
{
	return c != ' ' && as_isprint(c);
}

int as_islower(int c)
{
	return c >= 'a' && c <= 'z';
}

int as_isprint(int c)
{
	return !as_iscntrl(c);
}

int as_ispunct(int c)
{
	return (c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`');
}

int as_isspace(int c)
{
	return (c >= 0xA && c <= 0xD) || as_isblank(c);
}

int as_isupper(int c)
{
	return c >= 'A' && c <= 'Z';
}

int as_isxdigit(int c)
{
	c = as_tolower(c);
	return as_isdigit(c) || (c >= 'a' && c <= 'f');
}

int as_isblank(int c)
{
	return c == '\t' || c == ' ';
}

int as_tolower(int c)
{
	if(as_isupper(c))
		return c - 'A' + 'a';

	return c;
}

int as_toupper(int c)
{
	if(as_islower(c))
		return c - 'a' + 'A';

	return c;
}
