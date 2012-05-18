#include <sysdep.h>

#undef LOAD_ARG4_TYPE
#define LOAD_ARG4_TYPE long long int

#include <sysdeps/unix/sysv/linux/wordsize-64/preadv.c>
