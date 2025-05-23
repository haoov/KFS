#include "klib.h"

void __attribute__((noreturn)) halt_cpu(void) {
	asm volatile("cli");

	while (true) {
		asm volatile("hlt");
	}

	__builtin_unreachable();
}
