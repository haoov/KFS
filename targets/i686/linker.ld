ENTRY(_start)

SECTIONS
{
	. = 0x00000800;
	.gdt : {
		gdt = .;
		*(.gdt)
	}

	. = 1M;
	_kernel_start = .;

	.boot ALIGN(4K) :
	{
		KEEP(*(.header))
	}

	.text ALIGN(4K) :
	{
		*(.text)
	}

	.rodata ALIGN(4K) :
	{
		*(.rodata)
	}

	.data ALIGN(4K) :
	{
		*(.data)
	}

	.bss ALIGN(4K) :
	{
		*(.bss)
	}

	_kernel_end = .;
}
