#include "stddef.h"
#include "stdint.h"

void	*memset(void *ptr, int val, size_t count)
{
	uint8_t	*bptr;

	bptr = (uint8_t *) ptr;
	for (size_t i = 0; i < count; ++i)
	{
		bptr[i] = (uint8_t) val;
	}
}
