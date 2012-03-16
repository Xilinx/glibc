/* Determine the wordsize from the preprocessor defines.  __LP64__ is
   defined for 64bit since GCC 3.3.  For GCC older than GCC 3.3, if
   __x86_64__ is defined, it must be 64bit.  */
#ifndef __GNUC_PREREQ
# if defined __GNUC__ && defined __GNUC_MINOR__
#  define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
# else
#  define __GNUC_PREREQ(maj, min) 0
# endif
#endif

#if defined __x86_64__ && (__LP64__ || !__GNUC_PREREQ (3, 3))
# define __WORDSIZE	64
# define __WORDSIZE_COMPAT32	1
#else
# define __WORDSIZE	32
#endif
