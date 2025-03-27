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
	set_scr_color((PCOLOR_BLACK << 4) | PCOLOR_GREEN);
	kclear();
	kprint("Welcome to our KFS project!\n");
	switch_scr(1);
	set_scr_color((PCOLOR_BLACK << 4) | PCOLOR_RED);
	kclear();
	kprint("Screen 2\n");
	switch_scr(0);
	print_stack(20);
}
