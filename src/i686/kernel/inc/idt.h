#ifndef IDT_H
#define IDT_H

#include "klib.h"

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
	uint32_t	base;
} __attribute__((packed));

extern void	idt_load(void);
void		idt_set_entry(uint8_t n, uint32_t b, uint16_t s, uint8_t f);
void		idt_install(void);

#endif
