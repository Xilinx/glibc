/* Multiple versions of strcasestr
   All versions must be listed in ifunc-impl-list.c.  */

#include "init-arch.h"

#ifndef __x86_64__
# define __strcasestr_sse2 __strcasestr_ia32
#endif

#define STRCASESTR __strcasestr_sse2

#include "string/strcasestr.c"

extern char *__strcasestr_sse42 (const char *, const char *) attribute_hidden;
extern __typeof (__strcasestr_sse2) __strcasestr_sse2 attribute_hidden;

libc_ifunc (__strcasestr,
	    HAS_SSE4_2 ? __strcasestr_sse42 : __strcasestr_sse2);
