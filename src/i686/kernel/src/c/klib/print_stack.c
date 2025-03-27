#include "intf.h"

uint32_t	*get_esp()
{
	uint32_t	*esp;

	__asm__ volatile (
		".intel_syntax noprefix\n\t"
		"mov %0, esp\n\t"
		".att_syntax prefix"
		: "=r" (esp)
		:
		:
	);

	return esp;
}

void	print_esp(uint32_t *esp, uint32_t depth)
{
	kprint("\n");
	kprint("Stack trace:\n");
	for (uint32_t i = 0; i < depth; ++i)
	{
		kprint("[esp + ");
		kprint_hex(i);
		kprint("]: ");
		kprint_hex(esp[i]);
		kprint("\n");
	}
}

void	print_stack(void)
{
	uint32_t	*esp = NULL;

	esp = get_esp();
	print_esp(esp, 20);
}
