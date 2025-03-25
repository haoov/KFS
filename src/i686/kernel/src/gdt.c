#include "gdt.h"

struct gdt_entry	gdt[3];
struct gdt_ptr		gdtp;

void	gdt_set_entry(int n, uint32_t b, uint32_t l, uint8_t a, uint8_t g)
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
	gdtp.limit = (sizeof (struct gdt_entry) * 3) - 1;
	gdtp.base = (uint32_t)&gdt;

	gdt_set_entry(0, 0, 0, 0, 0);

	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x93, 0xCF);

	(void)gdt_flush();
}
