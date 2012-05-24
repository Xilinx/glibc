abi-list-variants := 32 64
abi-list-32-options := -U__powerpc64__
abi-list-32-condition := __WORDSIZE == 32
abi-list-64-options := -D__powerpc64__
abi-list-64-condition := __WORDSIZE == 64

ifeq ($(subdir),rt)
librt-routines += rt-sysdep
endif

ifeq ($(subdir),stdlib)
gen-as-const-headers += ucontext_i.sym
endif

ifeq ($(subdir),elf)
sysdep_routines += dl-vdso
endif
