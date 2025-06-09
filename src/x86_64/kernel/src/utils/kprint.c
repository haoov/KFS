#include "intf.h"

void kprint_str(const char *str) {
	for (uint32_t i = 0; str[i]; ++i)
	{
		kprint_char(str[i]);
	}
}

void kprint_hex(uint64_t n) {
    
    char hex_digits[] = "0123456789ABCDEF";
    char buffer[19];
    buffer[0] = '0';
    buffer[1] = 'x';
    
    for (int i = 0; i < 16; i++) {
        int shift = 60 - (i * 4);
        int digit = (n >> shift) & 0xF;
        buffer[2 + i] = hex_digits[digit];
    }
    buffer[18] = '\0';
    
    kprint_str(buffer);
}

void kprint_int(int n) {
	char	buffer[11];
	int		i = 0;
	bool	neg;

	neg = false;
	if (!n)
	{
		kprint_char('0');
	}
	if (n < 0)
	{
		neg = true;
		n = -n;
	}
	while (n != 0)
	{
		buffer[i++] = '0' + (n % 10);
		n /= 10;
	}
	if (neg)
	{
		kprint_char('-');
	}
	while (i > 0)
	{
		kprint_char(buffer[--i]);
	}
}
