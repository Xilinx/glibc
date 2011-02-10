/* ARM is (usually) little-endian but with a big-endian FPU.  */

#ifndef _ENDIAN_H
# error "Never use <bits/endian.h> directly; include <endian.h> instead."
#endif

#ifdef __ARMEB__
#define __BYTE_ORDER __BIG_ENDIAN
#else
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#ifdef __VFP_FP__
#define __FLOAT_WORD_ORDER __BYTE_ORDER
#else
#define __FLOAT_WORD_ORDER __BIG_ENDIAN
#endif
