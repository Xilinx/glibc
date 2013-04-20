ifeq ($(subdir),string)
sysdep_routines += memcpy-power7 memcpy-a2 memcpy-power6 memcpy-cell \
		   memcpy-power4 memcpy-ppc32 memcmp-power7 memcmp-power4 \
		   memcmp-ppc32 memset-power7 memset-power6 memset-power4 \
		   memset-ppc32 strnlen-power7 strnlen-c strlen-power7 \
		   strlen-ppc32 strnlen-power7 strnlen-c strcasecmp-power7 \
		   strcasecmp_l-power7 strncmp-power7 strncmp-power4 \
		   strncmp-ppc32 mempcpy-power7 mempcpy-c \
		   memchr-power7 memchr-c memrchr-power7 memrchr-c \
		   strncase-power7 strncase_l-power7 rawmemchr-power7 \
		   rawmemchr-c strchrnul-power7 strchrnul-c strchr-power7 \
		   strchr-ppc32 wcschr-power7 wcschr-power6 wcschr-c \
		   wcsrchr-power7 wcsrchr-power6 wcsrchr-c wcscpy-power7 \
		   wcscpy-power6 wcscpy-c

CFLAGS-strncase-power7.c += -mcpu=power7 -funroll-loops
CFLAGS-strncase_l-power7.c += -mcpu=power7 -funroll-loops
CFLAGS-wcschr-power7.c += -mcpu=power7
CFLAGS-wcschr-power6.c += -mcpu=power6
CFLAGS-wcsrchr-power7.c += -mcpu=power7
CFLAGS-wcsrchr-power6.c += -mcpu=power6
CFLAGS-wcscpy-power7.c += -mcpu=power7
CFLAGS-wcscpy-power6.c += -mcpu=power6
endif
