#include "intf.h"
#include "kernel.h"

static volatile bool panic = false;

__attribute__((noreturn))
void  kernel_panic(const char *msg, struct regs *regs) {
	uint32_t color = (PCOLOR_BLACK << 4) | PCOLOR_GREEN;

	if (panic) asm volatile("cli");

	set_scr_color((PCOLOR_BLACK << 4)  | PCOLOR_RED);

	kprint("KERNEL PANIC\n");
	kprint("%s. System Halted!\n", msg);
	halt_cpu();
}
