#include <sysdep.h>

#if defined SHARED && !defined NOT_IN_libc
# undef EALIGN
# define EALIGN(name, alignt, words)				\
  .globl C_SYMBOL_NAME(__memset_ppc32);				\
  .type C_SYMBOL_NAME(__memset_ppc32),@function;		\
  .align ALIGNARG(alignt);					\
  EALIGN_W_##words;						\
  C_LABEL(__memset_ppc32)					\
  cfi_startproc;

# undef END
# define END(name)						\
  cfi_endproc;							\
  ASM_SIZE_DIRECTIVE(__memset_ppc32)

# undef libc_hidden_builtin_def
# define libc_hidden_builtin_def(name)				\
    .globl __GI_memset; __GI_memset = __memset_ppc32
#endif

#include <sysdeps/powerpc/powerpc32/memset.S>
