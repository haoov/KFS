#include "intf.h"
#include "kernel.h"

static volatile bool panic = false;

char	*except_msg[] =
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

__attribute__((noreturn))
void  kernel_panic(struct regs *regs) {
	uint32_t color = (PCOLOR_BLACK << 4) | PCOLOR_GREEN;

	if (panic) asm volatile("cli");

	set_scr_color((PCOLOR_BLACK << 4)  | PCOLOR_RED);

	kprint("KERNEL PANIC\n");
	if (regs->int_no < 32)
	{
		kprint(except_msg[regs->int_no]);
		kprint(" Exception. System Halted!\n");
		halt_cpu();
	}
}
