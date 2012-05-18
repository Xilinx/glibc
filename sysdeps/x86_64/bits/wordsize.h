/* Determine the wordsize from the preprocessor defines.  */

#if defined __x86_64__ && !defined __ILP32__
# define __WORDSIZE	64
#else
# define __WORDSIZE	32
#endif

#ifdef __x86_64__
# define __WORDSIZE_TIME64_COMPAT32		1
# define __WORDSIZE_NO_STATVFSBUF_F_UNUSED	1
#endif
