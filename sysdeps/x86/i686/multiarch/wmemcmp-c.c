#ifndef NOT_IN_libc
# include <wchar.h>

# define WMEMCMP  __wmemcmp_generic

extern __typeof (wmemcmp) __wmemcmp_generic;
#endif

#include "wcsmbs/wmemcmp.c"
