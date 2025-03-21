#ifndef IDT_H
#define IDT_H

#include "stdint.h"
#include "stddef.h"

struct idt_entry
{
	uint16_t	base_lo;
	uint16_t	seg;
	uint8_t		res;
	uint8_t		flags;
	uint16_t	base_hi;
} __attribute__((packed));

struct idt_ptr
{
	uint16_t	limit;
	size_t		base;
} __attribute__((packed));

void	idt_load(void);
void	idt_set_entry(uint8_t num, uint32_t base, uint16_t seg, uint8_t flags);
void	idt_install(void);

#endif

