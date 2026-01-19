/* declare constants for multiboot header */
.set ALIGN, 1<<0                /* align loaded modules on page boundaries */
.set MEMINFO, 1<<1              /* provide memory map */
.set FLAGS, ALIGN | MEMINFO     /* multiboot flags field */
.set MAGIC, 0x1BADB002          /* let bootloader find header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum to prove we are multiboot */

/* declare multiboot header; must be in first 8KiB of kernel file */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* GDT */
.section .rodata
.align 8
gdt_data:
	.long 0x00000000 /* null descriptor */
	.long 0x00000000

	/* code descriptor: base 0x0, limit 0xFFFFF, 4KiB granularity */
	.word 0xFFFF	 /* limit low */
	.word 0x0000     /* base low */
	.byte 0x00	 /* base middle */
	.byte 0x9A	 /* access */
	.byte 0xCF	 /* granularity */
	.byte 0x00	 /* base high */

	/* data descriptor: base 0x0, limit 0xFFFFF, 4KiB granularity */
	.word 0xFFFF	 /* limit low */
	.word 0x0000	 /* base low */
	.byte 0x00	 /* base middle */
	.byte 0x92	 /* access */
	.byte 0xCF	 /* granularity */
	.byte 0x00	 /* base high */

end_of_gdt:
gdt_descriptor:
	.word end_of_gdt - gdt_data - 1	/* limit (size of GDT) */
	.long gdt_data			/* base of GDT */

/* 16KiB stack, 16-byte aligned */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* kernel entry point */
.section .text
.globl _start
.type _start, @function
_start:
        /* GRUB loaded into 32-bit protected mode, so interrupts + paging disabled */
        /* set up GDT */
        cli                 /* ensure interrupts off even though they should be already */
        lgdt gdt_descriptor /* load GDT into GDTR */
        ljmp $0x08, $1f     /* reload CS with the new code selector */
1:
        movw $0x10, %ax     /* data selector */
        movw %ax, %ds
        movw %ax, %ss
        movw %ax, %es
        movw %ax, %fs
        movw %ax, %gs

        /* set up stack; must be done in assembly, as C cannot function without a stack */
        mov $stack_top, %esp

        /* call global constructors */
        call _init

        /* transfer control to the main kernel */
        call kernel_main

        /* hang if kernel_main unexpectedly returns */
        cli
1:      hlt
        jmp 1b 

/* 
 * set size of _start symbol to current location '.' minus its start;
 * useful when debugging or when implementing call tracing
 */
.size _start, . - _start
