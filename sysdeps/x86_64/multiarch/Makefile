ifeq ($(subdir),string)

sysdep_routines += stpncpy-c strcmp-ssse3 strncmp-ssse3 \
		   strend-sse4 memcmp-sse4 memcpy-ssse3 mempcpy-ssse3 \
		   memmove-ssse3 memcpy-ssse3-back mempcpy-ssse3-back \
		   memmove-ssse3-back strcasecmp_l-ssse3 \
		   strncase_l-ssse3 strlen-sse4 strlen-sse2-no-bsf memset-x86-64 \
		   strcpy-ssse3 strncpy-ssse3 stpcpy-ssse3 stpncpy-ssse3 \
		   strcpy-sse2-unaligned strncpy-sse2-unaligned \
		   stpcpy-sse2-unaligned stpncpy-sse2-unaligned \
		   strcat-sse2-unaligned strncat-sse2-unaligned \
		   strcat-ssse3 strncat-ssse3 strlen-sse2-pminub \
		   strnlen-sse2-no-bsf strrchr-sse2-no-bsf strchr-sse2-no-bsf \
		   memcmp-ssse3
endif

ifeq ($(subdir),wcsmbs)
sysdep_routines += wmemcmp-sse4 wmemcmp-ssse3 wcscpy-ssse3
endif
