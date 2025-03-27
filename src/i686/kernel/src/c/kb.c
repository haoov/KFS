#include "klib.h"
#include "kb.h"
#include "ports.h"
#include "kernel.h"
#include "intf.h"
#include "irq.h"
#include "intf.h"

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

uint8_t	mod = 0;

void	kb_handler(struct regs *r)
{
	uint8_t	code;

	code = inb(0x60);
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
		else if (keymap[code] == '\b')
		{
			kdel_char();
		}
		else
		{
			if (mod & RSHIFTM || mod & LSHIFTM)
			{
				kprint_char(keymap[code + 100]);
			}
			else if (mod & ALTM)
			{
				switch_scr(keymap[code] - '0' - 1);
			}
			else
			{
				kprint_char(keymap[code]);
			}
		}
	}
}

void	kb_install(void)
{
	irq_install_handler(1, kb_handler);
}
