#include "idt.h"
#include "print.h"

void	kernel_main(void)
{
	(void)idt_install();
	(void)clear_scr();
	print_str("Hellooooo");
}
