#define AS_STRNLEN
#define strlen __strnlen
#define __strlen_sse2 __strnlen_sse2
#include "sysdeps/x86_64/strlen.S"
weak_alias(__strnlen,strnlen);
weak_alias(__strnlen,__GI_strnlen);

