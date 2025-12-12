ENTRY(_start)

SECTIONS
{
    /* QEMU virt starts RAM at 0x8000_0000 for RV32 */
    . = 0x80000000;

    .text : {
        *(.init*)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    }

    /* leave some headroom for future data; virt has RAM growing upward */
    _stack_top = 0x80400000;
}
