/* Determine the wordsize from the preprocessor defines.  __LP64__ is
   defined for 64-bit since GCC 3.3.  For compilers other than GCC 3.3
   or newer, we check if LONG_MAX > INT_MAX.  */

#include <features.h>

#if __GNUC_PREREQ (3, 3)
# if defined __x86_64__  && defined __LP64__
#  define __WORDSIZE	64
#  define __WORDSIZE_COMPAT32	1
# else
#  define __WORDSIZE	32
# endif
#else
#include <limits.h>
# if defined __x86_64__ && LONG_MAX > INT_MAX
#  define __WORDSIZE	64
#  define __WORDSIZE_COMPAT32	1
# else
#  define __WORDSIZE	32
# endif
#endif
