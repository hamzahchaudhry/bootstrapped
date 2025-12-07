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

/* 
 * linker script specifies _start as entry point to kernel.
 * bootloader jumps to this position once kernel is loaded, so don't return.
 */
.section .text
.global _start
.type _start, @function
_start:
    /* bootloader has loaded into 32-bit protected mode, so interrupts + paging disabled. */

    /* set up stack. must be done in assembly, as C cannot function without a stack. */
    mov $stack_top, %esp

   /* 
    * initialize crucial processor state before high-level kernel is entered.
    * GDT, paging, etc.
    * implement this later.
    */


    /* enter high-level kernel */
    call kernel_main

    /* if system has nothing more to do, put computer into infinite loop. */
    cli /* interrupts already disabled, but if we enable interrupts then return from kernel_main, have this as safety. */
1:  hlt /* wait for next interrupt to arrive, since they are disabled, this locks up computer. */
    jmp 1b /* jump to hlt if it ever wakes up due to non-maskable interrupt or due to system management mode */

/* 
 * set size of _start symbol to current location '.' minus its start.
 * useful when debugging or when implementing call tracing. 
 */
.size _start, . - _start
