ifeq ($(filter -DBOOTSTRAP_GCC,$(CFLAGS)),)
ifeq ($(filter -mabi=32,$(CC)),)
CC += -mabi=32
endif
else
CC += -D"_MIPS_SZPTR=32"
endif
