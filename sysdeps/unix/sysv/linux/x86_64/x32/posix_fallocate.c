#include <sysdep.h>

/* Since posix_fallocate is implemented by calling the fallocate
   system call with: fallocate (fd, 0, offset, len), we redefine
   arguments 3 (offset) and 4 (len) to be long long int to match:

   int fallocate(int fd, int mode, off_t offset, off_t len);
 */

#undef LOAD_ARG3_TYPE
#define LOAD_ARG3_TYPE long long int
#undef LOAD_ARG4_TYPE
#define LOAD_ARG4_TYPE long long int

#include <sysdeps/unix/sysv/linux/wordsize-64/posix_fallocate.c>
