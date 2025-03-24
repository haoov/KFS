#include "klib.h"

void	*memset(void *ptr, int val, uint32_t size)
{
	uint32_t	ui32val, w, nb, na, *ui32ptr;
	uint8_t		*ui8ptr;

	// Set the 4bytes of ui32val to val
	ui32val = (uint32_t)val;
	ui32val |= ui32val << 8;
	ui32val |= ui32val << 16;

	/* Computes how far we are from a 4bytes aligned address
	 * all multiple of 4 ends with 00 in binary
	 * ex: 11001001 & 11 = 01 = 1 so we are 3 (4 - 1) bytes from being aligned
	 * The last '& 0b11' operation is for the case when we are already aligned
	 * */
	nb = ((4 - ((uint32_t)ptr & 0b11)) & 0b11);
	nb = nb > size ? size : nb;

	// Set all unaligned bytes before and substract this number from the size
	size -= nb;
	ui8ptr = (uint8_t*)ptr;
	while (nb--)
	{
		*ui8ptr++ = (uint8_t)val;
	}

	// Main loop set bytes 4 by 4
	ui32ptr = (uint32_t*)ui8ptr;
	w = size >> 2;
	while (w--)
	{
		*ui32ptr++ = ui32val;
	}

	// Set all unaligned bytes after
	na = size & 0b11;
	ui8ptr = (uint8_t*)ui32ptr;
	while (na--)
	{
		*ui8ptr++ = (uint8_t)val;
	}

	return (ptr);
}
