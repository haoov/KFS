#include "print.h"
#include "ports.h"

struct chr	*buf = (struct chr *) 0xB8000;
uint8_t		col = 0;
uint8_t		row = 0;
uint8_t		color = PCOLOR_GREEN | PCOLOR_BLACK;

void	update_csr(void)
{
	uint16_t	idx;

	idx = SCR_BUF_IDX(col, row);
	outb(0x3D4, 14);
	outb(0x3D5, idx >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, idx);
}

void	clear_row(int r)
{
	struct chr	empty;

	empty.code = 32;
	empty.color = color;
	for (int c = 0; c < COLS_NUM; ++c)
	{
		buf[SCR_BUF_IDX(c, r)] = empty;
	}
}

void	clear_scr(void)
{
	for (int r = 0; r < ROWS_NUM; ++r)
	{
		(void)clear_row(r);
	}
	col = 0;
	row = 0;
	(void)update_csr();
}

void	scroll_down(void)
{
	struct chr	char_s;

	for (int r = 1; row < ROWS_NUM; ++r)
	{
		for (int c = 0; c < COLS_NUM; ++c)
		{
			char_s = buf[SCR_BUF_IDX(c, r)];
			buf[c + COLS_NUM * (r - 1)] = char_s;
		}
	}
}

void	print_nl(void)
{
	struct chr	char_s;
	uint16_t	csr_idx;

	col = 0;
	if (row < ROWS_NUM - 1)
	{
		++row;
	}
	else
	{
		(void)scroll_down();
		clear_row(ROWS_NUM - 1);
	}
	(void)update_csr();
}

void	print_char(const char c)
{
	struct chr	char_s;
	uint16_t	csr_idx;

	char_s.code = (uint8_t)c;
	char_s.color = color;
	csr_idx = SCR_BUF_IDX(col, row);
	if (c == '\n')
	{
		(void)print_nl();
		return;
	}
	if (col > COLS_NUM)
	{
		(void)print_nl();
	}
	buf[csr_idx] = char_s;
	++col;
	(void)update_csr();
}

void	print_str(const char *str)
{
	for (uint32_t i = 0; str[i]; ++i)
	{
		(void)print_char(str[i]);
	}
}
