ARCH      ?= riscv
CROSS     ?= riscv64-elf-
CC         = $(CROSS)gcc
CFLAGS     = -march=rv32i -mabi=ilp32 -Wall -O2 -ffreestanding -nostdlib -fno-builtin
LDFLAGS    = -T linker.ld -nostdlib -Wl,-melf32lriscv
OBJS = boot.o kernel.o

all: kernel.elf

kernel.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f kernel.elf $(OBJS)
