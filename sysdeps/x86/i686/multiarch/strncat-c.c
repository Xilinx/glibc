#define STRNCAT __strncat_generic
#ifdef SHARED
# undef libc_hidden_def
# define libc_hidden_def(name) \
  __hidden_ver1 (__strncat_generic, __GI___strncat, __strncat_generic);
#endif

#include "string/strncat.c"
