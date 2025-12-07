/* bootloader looks at this image and starts execution at symbol designated as entry point */
ENTRY(_start)

/* tell kernel where sections of object files will be put in final kernal image */
SECTIONS
{
    /* 
     * under BIOS, 1M start offset was effectively guaranteed.
     * UEFI made things complicated, and 2M is a safer place to load.
    */
    . = 2M;
    
    /* first put multiboot header, as it needs to be in first 8KiB */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* compiler may produce other sections, and will by default put them in a segment with same name. */

    /* ADD STUFF HERE AS NEEDED */
}
