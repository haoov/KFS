#ifndef KLIB_H
#define KLIB_H

#include "types.h"

#define CEIL_DIV(a, b) ((a + (b - 1)) / b)

void *memset(void *ptr, int val, uint64_t size);
void *memcpy(void *dst, void *src, uint64_t size);
uint8_t	strncmp(const char *s1, const char *s2, uint64_t n);
uint64_t strlen(const char *s);

#endif
