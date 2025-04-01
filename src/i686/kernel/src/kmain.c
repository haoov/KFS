#include "intf.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"
#include "kb.h"
#include "ksh.h"
#include "boot.h"
#include "vmm.h"
#include "pmm.h"

extern uint32_t kernel_start;

void	entry_msg()
{
	set_scr_color((PCOLOR_BLACK << 4) | PCOLOR_GREEN);
	kclear();
	kprint("      _                    ____   _____ \n");
	kprint("     | |                  / __ \\ / ____|\n");
	kprint("     | |_   _ _ __ __ _  | |  | | (___  \n");
	kprint(" _   | | | | | '__/ _` | | |  | |\\___ \\ \n");
	kprint("| |__| | |_| | | | (_| | | |__| |____) |\n");
	kprint(" \\____/ \\__,_|_|  \\__,_|  \\____/|_____/ \n");
	system_infos();
	kprint_char(NEWLINE);
}

void	kmain(mb_inf_t *mb_inf)
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	kb_install();
	__asm__ volatile ("sti");
	entry_msg();
	ksh_prompt();
	pmm_init((mmap_ent_t *)mb_inf->mmap_addr, mb_inf->mmap_length);
	vmm_init();
	void *addr = pmm_alloc_pages(3);
	kprint("3 Pages allocated at addr: %x\n", (uint32_t)addr);
	void *addr2 = pmm_alloc_pages(5);
	kprint("5 Pages allocated at addr: %x\n", (uint32_t)addr);
	pmm_free_pages(addr, 3);
	pmm_free_pages(addr, 5);
}
