#ifndef KLIB_H
#define KLIB_H

typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef uint8_t			bool;

#define NULL	(void*)0

#define true	1
#define false	0

#define CEIL_DIV(a, b) ((a + (b - 1)) / b)

void		*memset(void *ptr, int val, uint32_t size);
void		*memcpy(void *dst, void *src, uint32_t size);
void		print_stack(void);
uint32_t	strlen(const char *s);
uint8_t		strncmp(const char *s1, const char *s2, uint32_t n);

#endif
