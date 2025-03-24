#ifndef KLIB_H
#define KLIB_H

typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;

#define NULL	(void*)0

void	*memset(void *ptr, int val, uint32_t size);

#endif
