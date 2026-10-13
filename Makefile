CC = riscv64-unknown-elf-gcc
CFLAGS = -g -ffreestanding -O0
LDFLAGS = -nostdlib -Wl,--gc-sections -Wl,--build-id=none -Wl,-T,riscv64-virt.ld

ASFILES = crt0.s ns16550a.s

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(ASFILES) kernel.c -o kernel
