#include <as/private_.h>

/* Public API */

void as_free(void* ptr)
{
	if(!ptr)
		return;

	ulong* mem = &((ulong*) ptr)[-1];
	as_anonmunmap(mem, *mem);
}

void* as_malloc(ulong size)
{
	if(size == 0)
		return NULL;

	size = ALIGN(size + sizeof(ulong), as_getpagesize());
	ulong* mem = (ulong*) as_anonmmap(size);
	*mem = size;
	return (void*) &mem[1];
}

void* as_memcpy(void* dst, const void* src, ulong len)
{
	if(dst == src)
		return dst;

	char* cdst = (char*) dst;
	const char* csrc = (const char*) src;

	while(len-- && (*cdst++ = *csrc++));

	return dst;
}

void* as_memset(void* s, int c, ulong len)
{
	char* cs = (char*) s;

	while(len-- && (*cs++ = (char) c));

	return s;
}

void* as_memmove(void* dst, const void* src, ulong len)
{
	if(dst <= src)
		return as_memcpy(dst, src, len);

	char* cdst = (char*) dst + len;
	const char* csrc = (const char*) src + len;

	while(len-- && (*--cdst = *--csrc));

	return dst;
}
