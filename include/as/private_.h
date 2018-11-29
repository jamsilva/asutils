#ifndef AS_PRIVATE_H_INCLUDED_
#define AS_PRIVATE_H_INCLUDED_
#include <as/char.h>
#include <as/fdio.h>
#include <as/math.h>
#include <as/memory.h>
#include <as/pinfo.h>
#include <as/signal.h>
#include <as/string.h>
#include <as/time.h>
#include <as/trace.h>
#include <as/utils.h>

#ifndef NULL
	#define NULL 0L
#endif

#define ALIGN(v, align) ((ulong) (((v) + ((align) - 1)) & ~((align) - 1)))
#define SMAX(type)      (((type) 1) << (sizeof(type) * 8 - 1))
#define UMAX(type)      (~((type) 0))

#define TO_VA(ap, arg, ...) \
	va_list ap; \
	va_start(ap, arg); \
	__VA_ARGS__; \
	va_end(ap)

#endif
