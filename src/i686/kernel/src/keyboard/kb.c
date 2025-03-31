#include "klib.h"
#include "kb.h"
#include "ports.h"
#include "kernel.h"
#include "intf.h"
#include "irq.h"
#include "intf.h"
#include "ksh.h"

uint8_t	keymap[256] =
{
	0,		27,		'1',	'2',	'3',	'4',	'5',	'6',	'7',	'8',
	'9',	'0',	'-',	'=',	'\b',	'\t',	'q',	'w',	'e',	'r',
	't',	'y',	'u',	'i',	'o',	'p',	'[',	']',	'\n',	CTRLM,
	'a',	's',	'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
	'\'',	'`',	LSHIFTM,'\\',	'z',	'x',	'c',	'v',	'b',	'n',
	'm',	',',	'.',	'/',	RSHIFTM,'*',	ALTM,	' ',	0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		'-',	0,		0,		0,
	'+',	0,		0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		OPTM,	0,		0,		0,		0,		0,		0,		0,
	0,		0,		'!',	'@',	'#',	'$',	'%',	'^',	'&',	'*',
	'(',	')',	'_',	'+',	'\b',	'\t',	'Q',	'W',	'E',	'R',
	'T',	'Y',	'U',	'I',	'O',	'P',	'{',	'}',	0,		0,
	'A',	'S',	'D',	'F',	'G',	'H',	'J',	'K',	'L',	':',
	'"',	'~',	0,		'|',	'Z',	'X',	'C',	'V',	'B',	'N',
	'M',	'<',	'>',	'?',
};

uint8_t		mod = 0;
uint8_t		input_buffer[COLS_NUM - 3];
uint16_t	idx = 0;

void	reset_buffer(void)
{
	memset(input_buffer, 0, COLS_NUM - 3);
	idx = 0;
}

void	input_char(uint8_t c)
{
	if (idx < COLS_NUM - 4 || c == NEWLINE)
	{
		kprint_char(c);
		if (c == NEWLINE)
		{
			ksh_cmd((char*)input_buffer);
			reset_buffer();
			ksh_prompt();
		}
		else
		{
			input_buffer[idx++] = c;
		}
	}
}

void	del_char()
{
	input_buffer[idx] = '\0';
	if (idx)
	{
		--idx;
		kdel_char();
	}
}

void	kb_handler(struct regs *r)
{
	uint8_t	code;

	code = inb(KB_DATA_PORT);
	if (RELEASED(code))
	{
		mod &= ~keymap[code & 0b01111111];
	}
	else 
	{
		if (IS_MOD(code))
		{
			if (keymap[code] == ALTM)
			{
			}
			mod |= keymap[code];
		}
		else if (keymap[code] == BACKSPC)
		{
			del_char();
		}
		else
		{
			if (mod & RSHIFTM || mod & LSHIFTM)
			{
				input_char(keymap[code + 100]);
			}
			else
			{
				input_char(keymap[code]);
			}
		}
	}
}

void	kb_install(void)
{
	irq_install_handler(1, kb_handler);
	memset(input_buffer, 0, COLS_NUM - 3);
}
