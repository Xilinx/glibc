#define STRNCPY __strncpy_generic
#ifdef SHARED
# undef libc_hidden_builtin_def
# define libc_hidden_builtin_def(name) \
  __hidden_ver1 (__strncpy_generic, __GI_strncpy, __strncpy_generic);
#endif

#include "strncpy.c"
