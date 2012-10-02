ifeq ($(subdir),math)
libm-sysdep_routines += s_floor-c s_ceil-c s_floorf-c s_ceilf-c \
			s_rint-c s_rintf-c s_nearbyint-c s_nearbyintf-c

ifeq ($(have-mfma),yes)
libm-sysdep_routines += e_exp-fma e_log-fma e_pow-fma s_atan-fma \
			e_asin-fma e_atan2-fma s_sin-fma s_tan-fma \
			mplog-fma mpa-fma slowexp-fma slowpow-fma \
			sincos32-fma doasin-fma dosincos-fma \
			halfulp-fma mpexp-fma \
			mpatan2-fma mpatan-fma mpsqrt-fma mptan-fma

CFLAGS-doasin-fma.c = -mfma
CFLAGS-dosincos-fma.c = -mfma
CFLAGS-e_asin-fma.c = -mfma
CFLAGS-e_atan2-fma.c = -mfma
CFLAGS-e_exp-fma.c = -mfma
CFLAGS-e_log-fma.c = -mfma
CFLAGS-e_pow-fma.c = -mfma
CFLAGS-halfulp-fma.c = -mfma
CFLAGS-mpa-fma.c = -mfma
CFLAGS-mpatan-fma.c = -mfma
CFLAGS-mpatan2-fma.c = -mfma
CFLAGS-mpexp-fma.c = -mfma
CFLAGS-mplog-fma.c = -mfma
CFLAGS-mpsqrt-fma.c = -mfma
CFLAGS-mptan-fma.c = -mfma
CFLAGS-s_atan-fma.c = -mfma
CFLAGS-sincos32-fma.c = -mfma
CFLAGS-slowexp-fma.c = -mfma
CFLAGS-slowpow-fma.c = -mfma
CFLAGS-s_sin-fma.c = -mfma
CFLAGS-s_tan-fma.c = -mfma
endif

ifeq ($(have-mfma4),yes)
libm-sysdep_routines += e_exp-fma4 e_log-fma4 e_pow-fma4 s_atan-fma4 \
			e_asin-fma4 e_atan2-fma4 s_sin-fma4 s_tan-fma4 \
			mplog-fma4 mpa-fma4 slowexp-fma4 slowpow-fma4 \
			sincos32-fma4 doasin-fma4 dosincos-fma4 \
			halfulp-fma4 mpexp-fma4 \
			mpatan2-fma4 mpatan-fma4 mpsqrt-fma4 mptan-fma4

CFLAGS-doasin-fma4.c = -mfma4
CFLAGS-dosincos-fma4.c = -mfma4
CFLAGS-e_asin-fma4.c = -mfma4
CFLAGS-e_atan2-fma4.c = -mfma4
CFLAGS-e_exp-fma4.c = -mfma4
CFLAGS-e_log-fma4.c = -mfma4
CFLAGS-e_pow-fma4.c = -mfma4
CFLAGS-halfulp-fma4.c = -mfma4
CFLAGS-mpa-fma4.c = -mfma4
CFLAGS-mpatan-fma4.c = -mfma4
CFLAGS-mpatan2-fma4.c = -mfma4
CFLAGS-mpexp-fma4.c = -mfma4
CFLAGS-mplog-fma4.c = -mfma4
CFLAGS-mpsqrt-fma4.c = -mfma4
CFLAGS-mptan-fma4.c = -mfma4
CFLAGS-s_atan-fma4.c = -mfma4
CFLAGS-sincos32-fma4.c = -mfma4
CFLAGS-slowexp-fma4.c = -mfma4
CFLAGS-slowpow-fma4.c = -mfma4
CFLAGS-s_sin-fma4.c = -mfma4
CFLAGS-s_tan-fma4.c = -mfma4
endif

ifeq ($(config-cflags-sse2avx),yes)
libm-sysdep_routines += e_exp-avx e_log-avx s_atan-avx \
			e_atan2-avx s_sin-avx s_tan-avx \
			mplog-avx mpa-avx slowexp-avx \
			mpexp-avx

CFLAGS-e_atan2-avx.c = -msse2avx -DSSE2AVX
CFLAGS-e_exp-avx.c = -msse2avx -DSSE2AVX
CFLAGS-e_log-avx.c = -msse2avx -DSSE2AVX
CFLAGS-mpa-avx.c = -msse2avx -DSSE2AVX
CFLAGS-mpexp-avx.c = -msse2avx -DSSE2AVX
CFLAGS-mplog-avx.c = -msse2avx -DSSE2AVX
CFLAGS-s_atan-avx.c = -msse2avx -DSSE2AVX
CFLAGS-s_sin-avx.c = -msse2avx -DSSE2AVX
CFLAGS-slowexp-avx.c = -msse2avx -DSSE2AVX
CFLAGS-s_tan-avx.c = -msse2avx -DSSE2AVX
endif
endif
