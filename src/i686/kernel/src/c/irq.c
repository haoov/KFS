#include "irq.h"
#include "ports.h"
#include "idt.h"

void	*irq_routines[16] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

uint32_t	irq_subs[16] =
{
	(uint32_t)&irq0,
	(uint32_t)&irq1,
	(uint32_t)&irq2,
	(uint32_t)&irq3,
	(uint32_t)&irq4,
	(uint32_t)&irq5,
	(uint32_t)&irq6,
	(uint32_t)&irq7,
	(uint32_t)&irq8,
	(uint32_t)&irq9,
	(uint32_t)&irq10,
	(uint32_t)&irq11,
	(uint32_t)&irq12,
	(uint32_t)&irq13,
	(uint32_t)&irq14,
	(uint32_t)&irq15
};

void	irq_install_handler(int irq, void (*handler)(struct regs *r))
{
	irq_routines[irq] = handler;
}

void	irq_uninstall_handler(int irq)
{
	irq_routines[irq] = 0;
}

void	irq_remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

void	irq_install(void)
{
	irq_remap();
	for (uint32_t i = 0; i < 16; ++i)
	{
		idt_set_entry(i + 32, irq_subs[i], 0x08, 0x8E);
	}
}

void	irq_handler(struct regs *r)
{
	void	(*handler)(struct regs *);

	handler = irq_routines[r->int_no - 32];
	if (handler)
	{
		handler(r);
	}
	if (r->int_no >= 40)
	{
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);
}
