#include "ports.h"

/* send the byte 'byte' to the I/O port 'port'
 * */
void outb(uint16_t port, uint8_t byte)
{
	__asm__ volatile
	(
		".intel_syntax noprefix\n\t"
		"outb dx, al\n\t"
		".att_syntax prefix"
		:
		: "d" (port), "a" (byte)
		: "memory"
	);
}
