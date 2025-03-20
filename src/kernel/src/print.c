#include "print.h"

void	clear_row(int row, struct screen *screen)
{
	struct character	empty;

	empty.code = 32;
	empty.color = screen->cursor.color;
	for (int col = 0; col < COLS_NUM; ++col)
	{
		screen->buffer[col + COLS_NUM * row] = empty;
	}
}

void	clear_screen(struct screen *screen)
{
	for (int row = 0; row < ROWS_NUM; ++row)
	{
		clear_row(row, screen);
	}
}

void	print_nl(struct screen *screen)
{
	struct character	char_s;
	uint16_t			cursor_pos;

	cursor_pos = screen->cursor.col + COLS_NUM * screen->cursor.row;
	screen->cursor.col = 0;
	if (screen->cursor.row > ROWS_NUM)
	{
		for (int row = 1; row < ROWS_NUM; ++row)
		{
			for (int col = 0; col < COLS_NUM; ++col)
			{
				char_s = screen->buffer[cursor_pos];
				screen->buffer[cursor_pos - COLS_NUM];
			}
		}
		clear_row(COLS_NUM - 1, screen);
	}
	else
	{
		++screen->cursor.row;
	}
}

void	print_char(const char c, struct screen *screen)
{
	struct character	char_s;
	uint16_t			cursor_pos;

	char_s.code = (uint8_t)c;
	char_s.color = screen->cursor.color;
	cursor_pos = screen->cursor.col + COLS_NUM * screen->cursor.row;
	if (c == '\n')
	{
		print_nl(screen);
		return;
	}
	if (screen->cursor.col > COLS_NUM)
	{
		print_nl(screen);
	}
	screen->buffer[cursor_pos] = char_s;
	++screen->cursor.col;
}

void	print_str(const char *str, struct screen *screen)
{
	for (size_t i = 0; str[i]; ++i)
	{
		print_char(str[i], screen);
	}
}
