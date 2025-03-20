#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include <stddef.h>

#define COLS_NUM	80
#define ROWS_NUM	25

#define PCOLOR_BLACK		0x0
#define PCOLOR_BLUE			0x1
#define PCOLOR_GREEN		0x2
#define PCOLOR_CYAN			0x3
#define PCOLOR_RED			0x4
#define PCOLOR_MAGENTA		0x5
#define PCOLOR_BROWN		0x6
#define PCOLOR_LIGHT_GRAY	0x7
#define PCOLOR_LIGHT_BLUE	0x8
#define PCOLOR_LIGHT_GREEN	0x9
#define PCOLOR_LIGHT_CYAN	0xA
#define PCOLOR_LIGHT_RED	0xB
#define PCOLOR_PINK			0xC
#define PCOLOR_YELLOW		0xD
#define PCOLOR_WHITE		0xE

struct character
{
	uint8_t	code;
	uint8_t	color;
};

struct cursor
{
	uint8_t	col;
	uint8_t	row;
	uint8_t	color;
};

struct screen
{
	struct character	*buffer;
	struct cursor		cursor;
};

void	clear_screen(struct screen *screen);
void	print_nl(struct screen *screen);
void	print_char(const char c, struct screen *screen);
void	print_str(const char *str, struct screen *screen);

#endif

