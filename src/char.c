#include <as/private_.h>

/* Public API */

int _(ctoi)(int c)
{
	c = _(tolower)(c);

	if(_(islower)(c))
		return c - 'a' + 10;

	if(_(isdigit)(c))
		return c - '0';

	return -1;
}

int _(isalnum)(int c)
{
	return _(isalpha)(c) || _(isdigit)(c);
}

int _(isalpha)(int c)
{
	return _(isupper)(c) || _(islower)(c);
}

int _(iscntrl)(int c)
{
	return c == 0x7F || (c >= '\0' && c <= 0x1F);
}

int _(isdigit)(int c)
{
	return c >= '0' && c <= '9';
}

int _(isgraph)(int c)
{
	return c != ' ' && _(isprint)(c);
}

int _(islower)(int c)
{
	return c >= 'a' && c <= 'z';
}

int _(isprint)(int c)
{
	return !_(iscntrl)(c);
}

int _(ispunct)(int c)
{
	return (c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`');
}

int _(isspace)(int c)
{
	return (c >= 0xA && c <= 0xD) || _(isblank)(c);
}

int _(isupper)(int c)
{
	return c >= 'A' && c <= 'Z';
}

int _(isxdigit)(int c)
{
	c = _(tolower)(c);
	return _(isdigit)(c) || (c >= 'a' && c <= 'f');
}

int _(isblank)(int c)
{
	return c == '\t' || c == ' ';
}

int _(tolower)(int c)
{
	if(_(isupper)(c))
		return c - 'A' + 'a';

	return c;
}

int _(toupper)(int c)
{
	if(_(islower)(c))
		return c - 'a' + 'A';

	return c;
}
