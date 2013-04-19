ifeq ($(subdir),string)
sysdep_routines += memcpy-power7 memcpy-a2 memcpy-power6 memcpy-cell \
		   memcpy-power4 memcpy-ppc32 memcmp-power7 memcmp-power4 \
		   memcmp-ppc32 memset-power7 memset-power6 memset-power4 \
		   memset-ppc32 strnlen-power7 strnlen-c strlen-power7 \
		   strlen-ppc32 strnlen-power7 strnlen-c strcasecmp-power7 \
		   strcasecmp_l-power7 mempcpy-power7 mempcpy-c \
		   memchr-power7 memchr-c memrchr-power7 memrchr-c \
		   strncase-power7 strncase_l-power7 rawmemchr-power7 \
		   rawmemchr-c strchrnul-power7 strchrnul-c strchr-power7 \
		   strchr-ppc32

CFLAGS-strncase-power7.c += -mcpu=power7 -funroll-loops
CFLAGS-strncase_l-power7.c += -mcpu=power7 -funroll-loops
endif
