#ifndef BOOT_H
#define BOOT_H

#include "klib.h"

struct mb_inf
{
	uint32_t	flags;
	uint32_t	mem_lo;
	uint32_t	mem_up;
	uint32_t	boot_dev;
	uint32_t	cmd_line;
	uint32_t	mods_count;
	uint32_t	mods_addr;

	uint32_t	mmap_length;
	uint32_t	mmap_addr;
} __attribute__ ((packed));

struct mb_mmap_ent
{
	uint32_t	size;
	uint32_t	addr;
	uint32_t	len;
	uint32_t	type;
} __attribute__ ((packed));

#endif
