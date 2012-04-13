syscall-list-variants := 32bit 64bit
syscall-list-32bit-options := -D__i386__ -U__x86_64__
syscall-list-32bit-condition := !defined __x86_64__
syscall-list-64bit-options := -U__i386__ -U__ILP32__ -D__x86_64__ -D__LP64__
syscall-list-64bit-condition := defined __x86_64__

ifeq ($(subdir),misc)
sysdep_routines += ioperm iopl
sysdep_headers += sys/perm.h sys/reg.h sys/debugreg.h sys/io.h
endif

ifeq ($(subdir),stdlib)
sysdep_routines += __start_context
endif

ifeq ($(subdir),csu)
gen-as-const-headers += ucontext_i.sym
endif

ifeq ($(subdir),misc)
gen-as-const-headers += sigaltstack.sym
endif

ifeq ($(subdir),elf)
sysdep_routines += dl-vdso
endif
