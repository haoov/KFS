#include "intf.h"

void	kprint_str(const char *str)
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

    kprint_str(buffer);
}

void	kprint_int(int n)
{
	char	buffer[11];
	int		i = 0;
	bool	neg;

	neg = false;
	if (!n)
	{
		kprint_char('0');
	}
	if (i < 0)
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

void	kprint(const char *fmt, ...)
{
	void	*args = (void*)(&fmt + 1);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			++fmt;
			switch (*fmt)
			{
				case 'c':
					kprint_char(*(char*)args);
					args += sizeof (int);
					break;
				case 's':
					kprint_str(*(char**)args);
					args += sizeof (char*);
					break;
				case 'd':
					kprint_int(*(int*)args);
					args += sizeof (int);
					break;
				case 'x':
					kprint_hex(*(uint32_t*)args);
					args += sizeof (uint32_t);
					break;
				case '%':
					kprint_char('%');
					break;
				default:
					break;
			}
		}
		else
		{
			kprint_char(*fmt);
		}
		++fmt;
	}
}

