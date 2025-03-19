ENTRY(_start)

SECTIONS
{
    /* load the kernel from the adress 1 Mo (0x100000) */
    . = 1M;

    /* section code for the multiboot header */
    .multiboot :
    {
        KEEP(*(.multiboot))
    }

    /* section code for executable code bootloader's and kernel */
    .text :
    {
        *(.text)
    }

    /* section for data initialized */
    .data :
    {
        *(.data)
    }

    /* section for uninitialized variables */
    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    /* ensure that the memory adress is aligned to 16 bytes */
    . = ALIGN(16);
    _stack_top = .;

}