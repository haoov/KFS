#include "intf.h"

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
    char buffer[11];
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[10] = '\0';

    for (int i = 9; i >= 2; --i)
    {
        buffer[i] = hex_digits[n & 0xF];
        n >>= 4;
    }

    kprint(buffer);
}

