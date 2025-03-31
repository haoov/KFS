#include "klib.h"

uint8_t	strncmp(const char *s1, const char *s2, uint32_t n)
{
	uint32_t	i;

	i = 0;
	if (!s1 && !s2)
	{
		return 0;
	}
	else if (!s1 || !s2)
	{
		return 1;
	}
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
	{
		++i;
	}

	return (!(s1[i] == s2[i]));
}
