ifeq ($(subdir),string)
gen-as-const-headers += locale-defines.sym
sysdep_routines += bzero-sse2 memset-sse2 memcpy-ssse3 mempcpy-ssse3 \
		   memmove-ssse3 memcpy-ssse3-rep mempcpy-ssse3-rep \
		   memmove-ssse3-rep bcopy-ssse3 bcopy-ssse3-rep \
		   memset-sse2-rep bzero-sse2-rep strcmp-ssse3 \
		   strcmp-sse4 strncmp-c strncmp-ssse3 strncmp-sse4 \
		   memcmp-ssse3 memcmp-sse4 \
		   strlen-sse2 strlen-sse2-bsf strcpy-ssse3 \
		   strncpy-ssse3 stpcpy-ssse3 stpncpy-ssse3 strcpy-sse2 \
		   strncpy-sse2 stpcpy-sse2 stpncpy-sse2 strcat-ssse3 \
		   strcat-sse2 strncat-ssse3 strncat-sse2 \
		   strchr-sse2 strrchr-sse2 strchr-sse2-bsf strrchr-sse2-bsf \
		   memchr-sse2 memchr-sse2-bsf \
		   memrchr-sse2 memrchr-sse2-bsf memrchr-c \
		   rawmemchr-sse2 rawmemchr-sse2-bsf \
		   strnlen-sse2 strnlen-c \
		   strcasecmp_l-c strcasecmp-c strcasecmp_l-ssse3 \
		   strncase_l-c strncase-c strncase_l-ssse3 \
		   strcasecmp_l-sse4 strncase_l-sse4
endif

ifeq ($(subdir),wcsmbs)
sysdep_routines += wcscmp-sse2 wcscmp-c wcslen-sse2 wcslen-c \
		   wmemcmp-sse4 wmemcmp-ssse3 wcschr-sse2 \
		   wcschr-c wcsrchr-sse2 wcsrchr-c wcscpy-ssse3
endif

ifeq (mathyes,$(subdir)$(config-cflags-avx))
libm-sysdep_routines += s_fma-fma s_fmaf-fma
CFLAGS-s_fma-fma.c += -mavx -mfpmath=sse
CFLAGS-s_fmaf-fma.c += -mavx -mfpmath=sse
endif
