ifeq ($(subdir),csu)
aux += init-arch
tests += test-multiarch
gen-as-const-headers += ifunc-defines.sym
endif

ifeq ($(subdir),string)
sysdep_routines += strcasestr-nonascii strncat-c strncpy-c

ifeq (yes,$(config-cflags-sse4))
sysdep_routines += strcasestr-c strcspn-c strpbrk-c strspn-c strstr-c \
		   varshift
CFLAGS-strcasestr.c += -msse4
CFLAGS-strcasestr-nonascii.c += -msse4
CFLAGS-strcspn-c.c += -msse4
CFLAGS-strpbrk-c.c += -msse4
CFLAGS-strspn-c.c += -msse4
CFLAGS-strstr.c += -msse4
CFLAGS-varshift.c += -msse4
endif
endif

ifeq ($(subdir),wcsmbs)
sysdep_routines +=  wcscpy-c wmemcmp-c
endif
