#include "klib.h"

uint64_t	strlen(const char *s) {
	uint64_t	l;

	l = 0;
	while (s[l]) {
		++l;
	}

	return l;
}
