ENTRY(_start)

KERNEL_VMA = 0xC0000000 - 2M;

SECTIONS
{
	. = KERNEL_VMA;

	.boot : AT(ADDR(.boot) - KERNEL_VMA)
	{
		KEEP(*(.multiboot))
	}

	.text : AT(ADDR(.text) - KERNEL_VMA)
	{
		*(.text)
	}
}
