KERNEL_PHYSICAL_BASE_ADDR = 1M;
KERNEL_VIRTUAL_BASE_ADDR = 0xC0000000;

ENTRY(_start)

SECTIONS
{
	. = KERNEL_PHYSICAL_BASE_ADDR;

	.header ALIGN(4K) :
	{
		*(.header)
	}

	.boot ALIGN(4K) :
	{
		*(.boot)
	}

	. += KERNEL_VIRTUAL_BASE_ADDR;
	kernel_start = .;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE_ADDR)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_BASE_ADDR)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE_ADDR)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE_ADDR)
	{
		*(COMMON)
		*(.bss)
	}

	kernel_end = .;
}
