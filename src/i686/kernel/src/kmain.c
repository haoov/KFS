#include "intf.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"
#include "kb.h"

void	kmain(void)
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	kb_install();
	__asm__ volatile ("sti");
	set_scr_color((PCOLOR_BLACK << 4) | PCOLOR_GREEN, 0);
	set_scr_color((PCOLOR_WHITE << 4) | PCOLOR_BLACK, 1);
	kclear(0);
	kclear(1);
	kprint("Screen 1\n");
	switch_scr(1);
	kprint("Screen 2\n");
	switch_scr(0);
}
