#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "idt.h"

struct idt_entry	idt[256];
struct idt_ptr		idtp;

void	idt_load(void)
{
	__asm__ volatile
	(
		".intel_syntax noprefix\n\t"
		"lidt [%0]\n\t"
		".att_syntax prefix"
		:
		: "r" (&idtp)
		: "memory"
	);
}

void	idt_set_entry(uint8_t num, uint32_t base, uint16_t seg, uint8_t flags)
{
	idt[num].base_lo = base & 0xFFFF;
	idt[num].base_hi = base << 16;
	idt[num].seg = seg;
	idt[num].flags = flags;
}

void	idt_install(void)
{
	idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
	idtp.base = (size_t) &idt;
	(void)memset(&idt, 0, sizeof (struct idt_entry) * 256);
	(void)idt_load();
}
