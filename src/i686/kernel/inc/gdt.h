#ifndef GDT_H
#define GDT_H

#include "klib.h"

struct gdt_entry
{
	uint16_t	limit_lo;
	uint16_t	base_lo;
	uint8_t		base_mid;
	uint8_t		access;
	uint8_t		gran;
	uint8_t		base_hi;
} __attribute__((packed));

struct gdt_ptr
{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));

extern void	gdt_flush(void);
void		gdt_install(void);

#endif
