#include "print.h"

void	kernel_main(void)
{
	struct screen	screen;

	screen.buffer = (struct character*)0xB8000;
	screen.cursor.col = 0;
	screen.cursor.row = 0;
	screen.cursor.color = PCOLOR_GREEN | PCOLOR_BLACK << 4;
	clear_screen(&screen);
	print_nl(&screen);
	print_nl(&screen);
	print_nl(&screen);
	print_nl(&screen);
	print_nl(&screen);
	print_nl(&screen);
}
