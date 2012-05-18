#include <sysdep.h>

#undef LOAD_ARG2_TYPE
#define LOAD_ARG2_TYPE long long int
#undef LOAD_ARG3_TYPE
#define LOAD_ARG3_TYPE long long int

#include <sysdeps/unix/sysv/linux/wordsize-64/posix_fadvise.c>
