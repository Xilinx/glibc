# We don't need any header files.
abi-list-includes :=

# The default ABI is 32.
default-abi := 32

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
sysdep_routines += ioperm iopl vm86 call_pselect6 call_fallocate
sysdep_headers += sys/elf.h sys/perm.h sys/reg.h sys/vm86.h sys/debugreg.h sys/io.h
endif

ifeq ($(subdir),elf)
sysdep-others += lddlibc4
install-bin += lddlibc4
endif

ifeq ($(subdir),io)
sysdep_routines += call_sync_file_range
endif

ifeq ($(subdir),resource)
sysdep_routines += oldgetrlimit64
endif

ifeq ($(subdir),stdlib)
gen-as-const-headers += ucontext_i.sym
endif
