# We don't need any header files.
abi-list-includes :=

abi-list-variants := 32 64 x32

abi-list-32-options := -D__i386__ -U__x86_64__
abi-list-32-condition := !defined __x86_64__
abi-list-32-ld-soname := ld-linux.so.2
abi-list-64-options := -U__i386__ -D__x86_64__ -U__ILP32__ -D__LP64__
abi-list-64-condition := defined __x86_64__ && defined __LP64__
abi-list-64-ld-soname := ld-linux-x86-64.so.2
abi-list-x32-options := -U__i386__ -D__x86_64__ -D__ILP32__ -U__LP64__
abi-list-x32-condition := defined __x86_64__ && defined __ILP32__
abi-list-x32-ld-soname := ld-linux-x32.so.2

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
gen-as-const-headers += sigaltstack-offsets.sym
endif

ifeq ($(subdir),elf)
sysdep_routines += dl-vdso
endif
