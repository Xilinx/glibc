#include <sysdep.h>

/* Since posix_fallocate is implemented by calling the fallocate
   system call with: fallocate (fd, 0, offset, len), we redefine
   arguments 3 (offset) and 4 (len) to be long long int to match:

   int fallocate(int fd, int mode, off_t offset, off_t len);
 */

#undef LOAD_ARGS_4
#define LOAD_ARGS_4(a1, a2, a3, a4)			\
  LOAD_ARGS_TYPE_4 (long int, a1, long int, a2,		\
		    long long int, a3, long long int, a4)
#undef LOAD_REGS_4
#define LOAD_REGS_4					\
  LOAD_REGS_TYPE_4 (long int, long int, long long int,	\
		    long long int)

#include <sysdeps/unix/sysv/linux/wordsize-64/posix_fallocate.c>
