#ifndef NOT_IN_libc
# ifndef __x86_64__
#  define __wcscpy_sse2 __wcscpy_ia32
# endif
# define wcscpy  __wcscpy_sse2
#endif

#include "wcsmbs/wcscpy.c"
