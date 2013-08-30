#ifdef __CHKP__
# define MEMCPY mpx_memcpy_nobnd_nochk
# include "memcpy-ssse3-back.S"
#endif
