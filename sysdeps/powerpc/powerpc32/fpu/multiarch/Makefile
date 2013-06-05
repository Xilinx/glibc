ifeq ($(subdir),math)
libm-sysdep_routines += s_llrintf-power6 s_llrintf-power4 s_llrintf-ppc32 \
			s_llrint-power6 s_llrint-power4 s_llrint-ppc32 \
			s_llround-power6 s_llround-power5+ s_llround-power4 \
			s_llround-ppc32 w_sqrt-power5 w_sqrt-power4 \
			w_sqrt-ppc32 w_sqrtf-power5 w_sqrtf-power4 \
			w_sqrtf-ppc32

CFLAGS-s_llround.c = -fno-builtin-llroundf
endif
