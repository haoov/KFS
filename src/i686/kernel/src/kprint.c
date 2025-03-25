#include "intf.h"
#include "ports.h"
#include "klib.h"

struct chr	*intf_buffer = (struct chr *) 0xB8000;

struct scr	scrs[SCR_NUM];
uint8_t		cur_scr = 0;

void	update_csr(void)
{
	uint16_t	idx;
	struct scr	*scr;

	scr = &scrs[cur_scr];
	idx = SCR_BUF_IDX(scr->col, scr->row);
	outb(0x3D4, 14);
	outb(0x3D5, idx >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, idx);
}

void	update_intf(void)
{
	struct scr	scr;

	scr = scrs[cur_scr];
	(void)memcpy(intf_buffer, scr.buffer, SCR_SIZE * sizeof (struct chr));
	update_csr();
}

void	clear_row(int r, uint8_t scr_num)
{
	struct chr	empty;
	struct scr	*scr;

	scr = &scrs[scr_num];
	empty.code = ' ';
	empty.color = scr->color;
	for (int c = 0; c < COLS_NUM; ++c)
	{
		scr->buffer[SCR_BUF_IDX(c, r)] = empty;
		intf_buffer[SCR_BUF_IDX(c, r)] = empty;
	}
}

void	kclear(uint8_t scr_num)
{
	struct scr	*scr;

	scr = &scrs[scr_num];
	for (int r = 0; r < ROWS_NUM; ++r)
	{
		clear_row(r, scr_num);
	}
	scr->col = 0;
	scr->row = 0;
	update_csr();
}

void	scroll_down(void)
{
	struct chr	char_s;
	struct scr	*scr;

	scr = &scrs[cur_scr];
	for (int r = 1; r < ROWS_NUM; ++r)
	{
		for (int c = 0; c < COLS_NUM; ++c)
		{
			char_s = scr->buffer[SCR_BUF_IDX(c, r)];
			scr->buffer[c + COLS_NUM * (r - 1)] = char_s;
			intf_buffer[c + COLS_NUM * (r - 1)] = char_s;
		}
	}
}

void	print_nl(void)
{
	struct chr	char_s;
	struct scr	*scr;

	scr = &scrs[cur_scr];
	scr->col = 0;
	if (scr->row < ROWS_NUM - 1)
	{
		++scr->row;
	}
	else
	{
		scroll_down();
		clear_row(ROWS_NUM - 1, cur_scr);
	}
	update_csr();
}

void	kprint_char(const char c)
{
	struct chr	char_s;
	struct scr	*scr;
	uint16_t	idx;

	scr = &scrs[cur_scr];
	idx = SCR_BUF_IDX(scr->col, scr->row);
	char_s.code = (uint8_t)c;
	char_s.color = scr->color;
	if (c == '\n')
	{
		print_nl();
		return;
	}
	if (scr->col == COLS_NUM)
	{
		print_nl();
	}
	scr->buffer[idx] = char_s;
	intf_buffer[idx] = char_s;
	++scr->col;
	update_csr();
}

void	kdel_char(void)
{
	struct chr	empty;
	struct scr	*scr;

	scr = &scrs[cur_scr];
	empty.code = ' ';
	empty.color = scr->color;
	if (!scr->col)
	{
		if (scr->row)
		{
			--scr->row;
			scr->col = COLS_NUM - 1;
		}
	}
	else
	{
		--scr->col;
	}
	scr->buffer[SCR_BUF_IDX(scr->col, scr->row)] = empty;
	intf_buffer[SCR_BUF_IDX(scr->col, scr->row)] = empty;
	update_csr();
}

void	kprint(const char *str)
{
	for (uint32_t i = 0; str[i]; ++i)
	{
		kprint_char(str[i]);
	}
}

void	kprint_hex(uint32_t n)
{
    char hex_digits[] = "0123456789ABCDEF";
    char buffer[12];
    buffer[0] = '0';
    buffer[1] = 'x';
	buffer[10] = '\n';
    buffer[11] = '\0';

    for (int i = 9; i >= 2; --i)
    {
        buffer[i] = hex_digits[n & 0xF];
        n >>= 4;
    }

    kprint(buffer);
}

void	set_scr_color(uint8_t color, uint8_t scr_num)
{
	scrs[scr_num].color = color;
}

void	switch_scr(uint8_t idx)
{
	if (idx < 2)
	{
		cur_scr = idx;
		update_intf();
	}
}
