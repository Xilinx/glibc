/* Determine the wordsize from the preprocessor defines.  __LP64__ is
   defined for 64bit since GCC 3.3.  For GCC older than GCC 4.0, if
   __x86_64__ is defined, it must be 64bit.  */

#if defined __x86_64__ \
    && (defined __LP64__ || !defined __GNUC__ || __GNUC__ < 4)
# define __WORDSIZE	64
# define __WORDSIZE_COMPAT32	1
#else
# define __WORDSIZE	32
#endif
