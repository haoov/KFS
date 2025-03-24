#include "kernel.h"

void	kmain(void)
{
	kclear();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	__asm__ volatile ("sti");
}
