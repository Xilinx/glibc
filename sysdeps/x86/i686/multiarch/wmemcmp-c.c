#ifndef NOT_IN_libc
# include <wchar.h>

# ifndef __x86_64__
#  define __wmemcmp_sse2 __wmemcmp_ia32
# endif
# define WMEMCMP  __wmemcmp_sse2

extern __typeof (wmemcmp) __wmemcmp_sse2;
#endif

#include "wcsmbs/wmemcmp.c"
