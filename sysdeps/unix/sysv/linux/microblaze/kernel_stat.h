/* Definition of `struct stat' used in the kernel..  */
struct kernel_stat {
	unsigned long st_dev;
	unsigned long st_ino;
	unsigned int st_mode;
	unsigned int st_nlink;
	unsigned int st_uid;
	unsigned int st_gid;

	unsigned long st_rdev;	/* Device number, if device.  */
	unsigned long __pad1;
/* not sure if is used anywhere */
#define _HAVE___PAD1
	long st_size;		/* Size of file, in bytes.  */
	int st_blksize;		/* Optimal block size for I/O.  */
	int __pad2;
/* not sure if is used anywhere */
#define _HAVE___PAD2
	long st_blocks;		/* Number 512-byte blocks allocated. */
	struct timespec st_atim;
	struct timespec st_mtim;
	struct timespec st_ctim;
	unsigned int __unused4;
#define _HAVE___UNUSED4
	unsigned int __unused5;
#define _HAVE___UNUSED5
};

#define _HAVE_STAT___PAD1
#define _HAVE_STAT___PAD2

#define _HAVE_STAT___UNUSED4
#define _HAVE_STAT___UNUSED5

#define _HAVE_STAT_NSEC
#define _HAVE_STAT64___PAD1
#define _HAVE_STAT64___PAD2
#define _HAVE_STAT64_NSEC
