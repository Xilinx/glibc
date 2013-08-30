#ifdef __CHKP__
# define USE_AS_MEMMOVE
# define MEMCPY mpx_memmove_nobnd_nochk
# include "memcpy-ssse3-back.S"
#endif
