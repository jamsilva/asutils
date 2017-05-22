#include <as/private_.h>

/* Public API */

void* _(memcpy)(void* dst, const void* src, ulong len)
{
	if(dst == src)
		return dst;

	char* cdst = (char*) dst;
	const char* csrc = (const char*) src;

	while(len-- && (*cdst++ = *csrc++));

	return dst;
}

void* _(memset)(void* s, int c, ulong len)
{
	char* cs = (char*) s;

	while(len-- && (*cs++ = (char) c));

	return s;
}

void* _(memmove)(void* dst, const void* src, ulong len)
{
	if(dst <= src)
		return _(memcpy)(dst, src, len);

	char* cdst = (char*) dst + len;
	const char* csrc = (const char*) src + len;

	while(len-- && (*--cdst = *--csrc));

	return dst;
}
