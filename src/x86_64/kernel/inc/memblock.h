#ifndef MEMBLOCK_H
#define MEMBLOCK_H

#include "types.h"

#define ALIGN(v, n)	((v + (n - 1)) & ~(n - 1))
#define	INIT_MEMEBLOCK_REGIONS 128

struct memblock_region {
	uint64_t base;
	uint64_t size;
	uint32_t flags;
};

struct memblock_type {
	uint64_t count;
	uint64_t max;
	uint64_t total_size;
	struct memblock_region *regions;
};

struct memblock_manager {
	struct memblock_type memory;
	struct memblock_type reserved;
};

void memblock_init(uint64_t mb_infos_addr);

#endif
