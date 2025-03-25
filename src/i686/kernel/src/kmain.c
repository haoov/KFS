#include "intf.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"
#include "kb.h"

void	kmain(void)
{
	kclear();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	kb_install();
	__asm__ volatile ("sti");
}
