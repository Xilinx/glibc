/* This file selects the right generated file of `__stub_FUNCTION' macros
   based on the architecture being compiled for.  */

#ifdef __x86_64__
# include <gnu/stubs-64.h>
/* The following obsolete system calls are available in x86-64 kernel.
   But they are not supported in x32 kernel.  We always undef them for
   x86-64 and define them for x32.  It works with gnu/stubs-64.h
   generated during x32 glibc build as well as x86-64 glibc build.  */
# undef __stub_create_module
# undef __stub_get_kernel_syms
# undef __stub_nfsservctl
# undef __stub_query_module
# undef __stub_uselib
# ifdef __ILP32__
#  define __stub_create_module
#  define __stub_get_kernel_syms
#  define __stub_nfsservctl
#  define __stub_query_module
#  define __stub_uselib
# endif
#else
# include <gnu/stubs-32.h>
#endif
