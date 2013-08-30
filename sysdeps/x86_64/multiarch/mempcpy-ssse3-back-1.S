#ifdef __CHKP__
# define USE_AS_MEMPCPY
/* the version of mempcpy without ant checks or copying bounds. */
# define MEMCPY mpx_mempcpy_nobnd_nochk
# include "memcpy-ssse3-back.S"
#endif
