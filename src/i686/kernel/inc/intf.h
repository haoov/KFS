#ifndef INTF_H
#define INTF_H

#include "klib.h"

#define COLS_NUM	80
#define ROWS_NUM	25
#define SCR_SIZE	(COLS_NUM * ROWS_NUM)
#define SCR_NUM		2

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

/* Return the [c][r] index for the scr buffer */
#define SCR_BUF_IDX(c, r)	(c + r * COLS_NUM)

struct chr
{
	uint8_t	code;
	uint8_t	color;
};

struct scr
{
	struct chr	buffer[SCR_SIZE];
	uint8_t		col;
	uint8_t		row;
	uint8_t		color;
};

void	kclear(void);
void	kprint_char(const char c);
void	kdel_char(void);
void	kprint(const char *str);
void	kprint_hex(uint32_t n);
void	set_scr_color(uint8_t color);

#endif
