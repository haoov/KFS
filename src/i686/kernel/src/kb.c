#include "klib.h"
#include "kb.h"
#include "ports.h"
#include "kernel.h"
#include "intf.h"
#include "irq.h"

uint8_t	keymap[128] = {
	0,		27,		'1',	'2',	'3',	'4',	'5',	'6',	'7',	'8',
	'9',	'0',	'-',	'=',	'\b',	'\t',	'q',	'w',	'e',	'r',
	't',	'y',	'u',	'i',	'o',	'p',	'[',	']',	'\n',	0,
	'a',	's',	'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
	'\'',	'`',	0,		'\\',	'z',	'x',	'c',	'v',	'b',	'n',
	'm',	',',	'.',	'/',	0,		'*',	0,		' ',	0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		'-',	0,		0,		0,
	'+',	0,		0,		0,		0,		0,		0,		0,		0,		0,
	0,		0
};

void	kb_handler(struct regs *r)
{
	uint8_t	code;

	code = inb(0x60);
	if (code & 0b10000000)
	{

	}
	else 
	{
		if (keymap[code] == '\b')
		{
			kdel_char();
		}
		else
		{
			kprint_char(keymap[code]);
		}
	}
}

void	kb_install(void)
{
	irq_install_handler(1, kb_handler);
}
