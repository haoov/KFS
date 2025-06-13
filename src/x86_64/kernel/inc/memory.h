#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define ALIGN(v, n)	((v + (n - 1)) & ~(n - 1))

void memblock_init(uint64_t mb_infos_addr);

#endif
