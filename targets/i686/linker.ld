GDT_PHYSICAL_ADDR = 0x00000800;
KERNEL_PHYSICAL_ADDR = 1M;
HIGHER_HALF_ADDR = 0xC0000000;

ENTRY(_start)

SECTIONS
{
	. = GDT_PHYSICAL_ADDR;
	.gdt : {
		gdt = .;
		*(.gdt)
	}

	. = KERNEL_PHYSICAL_ADDR;

	.header ALIGN(4K) :
	{
		*(.header)
	}

	.boot ALIGN(4K) :
	{
		*(.boot)
	}

	. += HIGHER_HALF_ADDR;
	kernel_start = .;

	.text ALIGN(4K) : AT(ADDR(.text) - HIGHER_HALF_ADDR)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - HIGHER_HALF_ADDR)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - HIGHER_HALF_ADDR)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - HIGHER_HALF_ADDR)
	{
		*(COMMON)
		*(.bss)
	}

	kernel_end = .;
}
