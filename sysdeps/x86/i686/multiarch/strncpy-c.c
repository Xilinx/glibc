#ifndef __x86_64__
# define __strncpy_sse2 __strncpy_ia32
#endif

#define STRNCPY __strncpy_sse2
#ifdef SHARED
# undef libc_hidden_builtin_def
# ifdef __x86_64__
#  define libc_hidden_builtin_def(name) \
  __hidden_ver1 (__strncpy_sse2, __GI_strncpy, __strncpy_sse2);
# else
#  define libc_hidden_builtin_def(name) \
  __hidden_ver1 (__strncpy_ia32, __GI_strncpy, __strncpy_ia32);
# endif
#endif

#include "strncpy.c"
