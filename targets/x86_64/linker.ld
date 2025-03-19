KERNEL_VMA = 1M;

ENTRY(_start)

SECTIONS
{
	. = KERNEL_VMA;

	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.text :
	{
		*(.text)
	}
}
