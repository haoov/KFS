#include "timer.h"
#include "intf.h"

uint32_t	ticks = 0;

void	timer_phase(int hz)
{
	int div;

	div = 1193180 / hz;
	outb(0x43, 0x36);
	outb(0x40, div & 0xFF);
	outb(0x40, div >> 8);
}

void	timer_handler(struct regs *r)
{
	++ticks;
	if (!(ticks % TICK_RATE))
	{
		kprint("One second has passed\n");
	}
}

void	timer_install(void)
{
	timer_phase(TICK_RATE);
	irq_install_handler(0, timer_handler);
}
