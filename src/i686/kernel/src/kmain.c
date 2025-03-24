#include "kernel.h"

void	kmain(void)
{
	gdt_install();
	idt_install();
	isrs_install();
	kclear();
}
