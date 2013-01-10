#ifndef __x86_64__
# define __strncat_sse2 __strncat_ia32
#endif

#define STRNCAT __strncat_sse2
#ifdef SHARED
# undef libc_hidden_def
# ifdef __x86_64__
#  define libc_hidden_def(name) \
  __hidden_ver1 (__strncat_sse2, __GI___strncat, __strncat_sse2);
# else
#  define libc_hidden_def(name) \
  __hidden_ver1 (__strncat_ia32, __GI___strncat, __strncat_ia32);
# endif
#endif

#include "string/strncat.c"
