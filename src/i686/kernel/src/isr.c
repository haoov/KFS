#include "isr.h"

uint8_t	*except_msg[] =
{
	"Division by zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

uint32_t	isr_subs[32] =
{
	(uint32_t)&isr0,
	(uint32_t)&isr1,
	(uint32_t)&isr2,
	(uint32_t)&isr3,
	(uint32_t)&isr4,
	(uint32_t)&isr5,
	(uint32_t)&isr6,
	(uint32_t)&isr7,
	(uint32_t)&isr8,
	(uint32_t)&isr9,
	(uint32_t)&isr10,
	(uint32_t)&isr11,
	(uint32_t)&isr12,
	(uint32_t)&isr13,
	(uint32_t)&isr14,
	(uint32_t)&isr15,
	(uint32_t)&isr16,
	(uint32_t)&isr17,
	(uint32_t)&isr18,
	(uint32_t)&isr19,
	(uint32_t)&isr20,
	(uint32_t)&isr21,
	(uint32_t)&isr22,
	(uint32_t)&isr23,
	(uint32_t)&isr24,
	(uint32_t)&isr25,
	(uint32_t)&isr26,
	(uint32_t)&isr27,
	(uint32_t)&isr28,
	(uint32_t)&isr29,
	(uint32_t)&isr30,
	(uint32_t)&isr31,
};

void	isrs_install(void)
{
	for (uint32_t i = 0; i < 32; i++)
	{
		idt_set_entry(i, isr_subs[i], 0x08, 0x8E);
	}
}

void	fault_handler(struct regs *r)
{
	if (r->int_no < 32)
	{
		kprint(except_msg[r->int_no]);
		kprint(" Exception. System Halted!\n");
		for (;;);
	}
}
