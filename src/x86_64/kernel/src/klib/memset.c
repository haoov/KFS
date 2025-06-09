#include "klib.h"

void	*memset(void *ptr, int val, uint64_t size) {
	uint64_t	ui64val, w, nb, na, *ui64ptr;
	uint8_t		*ui8ptr;

	// Set the 4bytes of ui32val to val
	ui64val = (uint64_t)val;
	ui64val |= ui64val << 8;
	ui64val |= ui64val << 16;
	ui64val |= ui64val << 32;

	/* Computes how far we are from a 8bytes aligned address
	 * ex: 11001001 & 111 = 001 = 1 so we are 7 (8 - 1) bytes from being aligned
	 * The last '& 0b111' operation is for the case when we are already aligned
	 * */
	nb = ((4 - ((uint64_t)ptr & 0b111)) & 0b111);
	nb = nb > size ? size : nb;

	// Set all unaligned bytes before and substract this number from the size
	size -= nb;
	ui8ptr = (uint8_t*)ptr;
	while (nb--)
	{
		*ui8ptr++ = (uint8_t)val;
	}

	// Main loop set bytes 8 by 8
	ui64ptr = (uint64_t*)ui8ptr;
	w = size >> 3;
	while (w--)
	{
		*ui64ptr++ = ui64val;
	}

	// Set all unaligned bytes after
	na = size & 0b111;
	ui8ptr = (uint8_t*)ui64ptr;
	while (na--)
	{
		*ui8ptr++ = (uint8_t)val;
	}

	return (ptr);
}
