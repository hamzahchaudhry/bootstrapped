# Bootstrapped

Bootstrapped is a minimal 32-bit x86 operating system kernel derived from the
[OSDev Meaty Skeleton](https://wiki.osdev.org/Meaty_Skeleton).

The project is intended as an educational x86 kernel developed from scratch
to explore low-level systems programming.

## Screenshot
<img width="1920" height="500" alt="image" src="https://github.com/user-attachments/assets/0e1a747f-2338-4a1f-b0f5-ed4ce214cec9" />

## Directory Layout

```

.
├── kernel/                 # Kernel source
│   ├── arch/i386/          # x86-specific code
│   │   ├── boot.S          # Multiboot header and entry point
│   │   ├── crti.S / crtn.S # Runtime init/fini glue
│   │   ├── linker.ld       # Kernel linker script
│   │   ├── tty.c           # VGA terminal implementation
│   │   └── vga.h
│   ├── include/kernel/     # Public kernel headers
│   └── kernel/             # Core kernel logic
│
├── libc/                   # Freestanding C library (libk)
│   ├── include/            # Standard headers
│   ├── string/             # Memory/string functions
│   ├── stdio/              # printf / putchar / puts
│   └── stdlib/             # abort
│
├── sysroot/                # Build output (ignored by git)
├── build.sh                # Build kernel and libk
├── iso.sh                  # Build GRUB bootable ISO
├── qemu.sh                 # Run kernel in QEMU
└── README.md

````

---

## Building

### Requirements

- `i686-elf-gcc` cross compiler
- `make`
- `qemu-system-i386`

Optional (for ISO booting via GRUB):

- `grub-mkrescue`
- `grub-file`
- `xorriso`
- `mtools`

---

### Build

From the project root:

```sh
./build.sh
````

This produces:

* `libk.a` (kernel C library)
* `bootstrapped.kernel`
* populated `sysroot/`

---

### Run in QEMU

```sh
qemu-system-i386 -kernel bootstrapped.kernel
```

---

### Run via GRUB ISO

```sh
./iso.sh
./qemu.sh
```

---

## Inspiration & References

* [OSDev Wiki – Meaty Skeleton](https://wiki.osdev.org/Meaty_Skeleton)
* [OSDev Wiki](https://wiki.osdev.org)
* Linux, xv6
