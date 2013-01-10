#define STPNCPY __stpncpy_generic
#ifdef SHARED
#undef libc_hidden_def
#define libc_hidden_def(name) \
  __hidden_ver1 (__stpncpy_generic, __GI___stpncpy, __stpncpy_generic);
#endif

#include "stpncpy.c"
