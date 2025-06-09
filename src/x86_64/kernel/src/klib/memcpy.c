#include "klib.h"

void *memcpy(void *dst, void *src, uint64_t size) {
	uint8_t	*ui8ptr;

	ui8ptr = (uint8_t*)dst;

	for (uint64_t i = 0; i < size; ++i)
	{
		*ui8ptr++ = *(uint8_t*)src++;
	}

	return (dst);
}
