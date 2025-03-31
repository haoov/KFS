#include "klib.h"

uint32_t	strlen(const char *s)
{
	uint32_t	l;

	l = 0;
	while (s[l])
	{
		++l;
	}

	return l;
}
