#include "idt.h"

struct idt_entry	idt[256];
struct idt_ptr		idtp;

void	idt_set_entry(uint8_t n, uint32_t b, uint16_t s, uint8_t f)
{
	idt[n].base_lo = b & 0xFFFF;
	idt[n].base_hi = (b >> 16) & 0xFFFF;
	idt[n].res = 0;
	idt[n].seg = s;
	idt[n].flags = f;
}

void	idt_install(void)
{
	idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
	idtp.base = (uint32_t)&idt;
	(void)memset(&idt, 0, sizeof (struct idt_entry) * 256);
	idt_load();
}
