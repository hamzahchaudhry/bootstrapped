/* declare constants for multiboot header */
.set ALIGN, 1<<0                /* align loaded modules on page boundaries */
.set MEMINFO, 1<<1              /* provide memory map */
.set FLAGS, ALIGN | MEMINFO     /* multiboot flags field */
.set MAGIC, 0x1BADB002          /* let bootloader find header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum to prove we are multiboot */

/* declare multiboot header. must be in first 8KiB of kernel file. */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* 16KiB stack, 16-byte aligned */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* kernel entry point. */
.section .text
.global _start
.type _start, @function
_start:
        /* bootloader has loaded into 32-bit protected mode, so interrupts + paging disabled. */

        /* set up stack. must be done in assembly, as C cannot function without a stack. */
        mov $stack_top, %esp

        /* call global constructors.
        call _init

        /* transfer control to the main kernel. */
        call kernel_main

        /* hang if kernel_main unexpectedly returns */
        cli
1:      hlt
        jmp 1b 

/* 
 * set size of _start symbol to current location '.' minus its start.
 * useful when debugging or when implementing call tracing. 
 */
.size _start, . - _start
