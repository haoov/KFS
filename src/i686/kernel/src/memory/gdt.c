#include "gdt.h"
#include "intf.h"

__attribute__((section(".gdt")))
__attribute__((aligned(4)))
struct gdt_entry	gdt[7];

__attribute__((section(".gdt")))
__attribute__((aligned(4)))
struct gdt_ptr		gdtp;

void	gdt_set_gate(int n, uint32_t b, uint32_t l, uint8_t a, uint8_t g)
{
	gdt[n].base_lo = (b & 0xFFFF);
	gdt[n].base_mid = (b >> 16) & 0xFF;
	gdt[n].base_hi = (b >> 24) & 0xFF;

	gdt[n].limit_lo = (l & 0xFFFF);
	gdt[n].gran = (l >> 16) & 0x0F;

	gdt[n].gran |= (g & 0xF0);
	gdt[n].access = a;
}

void	gdt_install(void)
{
	gdtp.limit = (sizeof (struct gdt_entry) * 7) - 1;
	gdtp.base = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);

	// Kernel code segment
	gdt_set_gate(	1, 0, MEM_LIMIT,
					ACC_PRESENT | ACC_R0 | ACC_DESC | ACC_EXEC | ACC_CODE_RD,
					FLAG_GRAN4K | FLAG_32BIT);

	// Kernel data segment
	gdt_set_gate(	2, 0, MEM_LIMIT,
					ACC_PRESENT | ACC_R0 | ACC_DESC | ACC_DATA_WR,
					FLAG_GRAN4K | FLAG_32BIT);

	// Kernel stack segment
	gdt_set_gate(	3, 0, MEM_LIMIT,
					ACC_PRESENT | ACC_R0 | ACC_DESC | ACC_DATA_WR,
					FLAG_GRAN4K | FLAG_32BIT);

	// User code segment
	gdt_set_gate(	4, 0, MEM_LIMIT,
					ACC_PRESENT | ACC_R3 | ACC_DESC | ACC_EXEC | ACC_CODE_RD,
					FLAG_GRAN4K | FLAG_32BIT);

	// User data segment
	gdt_set_gate(	5, 0, MEM_LIMIT,
					ACC_PRESENT | ACC_R3 | ACC_DESC | ACC_DATA_WR,
					FLAG_GRAN4K | FLAG_32BIT);

	// User stack segment
	gdt_set_gate(	6, 0, MEM_LIMIT,
					ACC_PRESENT | ACC_R3 | ACC_DESC | ACC_DATA_WR,
					FLAG_GRAN4K | FLAG_32BIT);

	gdt_flush(); 
}
