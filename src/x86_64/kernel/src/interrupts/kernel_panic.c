#include "intf.h"
#include "kernel.h"

__attribute__((noreturn))
void  kernel_panic(const char *msg, struct interrupt_frame *frame) {
	set_scr_color((PCOLOR_BLACK << 4)  | PCOLOR_RED);

	kprint_str("KERNEL PANIC\n");
	kprint_str(msg);
	kprint_str(". System Halted!\n");
	halt_cpu();

	__builtin_unreachable();
}
