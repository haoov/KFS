#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include "types.h"

#define MB_MEMORY_AVAILABLE			1
#define MB_MEMORY_RESERVED			2
#define MB_MEMORY_ACPI_RECLAIMABLE	3
#define MB_MEMORY_NVS				4
#define MB_MEMORY_BADRAM			5
#define MB_TAG_TYPE_MMAP			6

struct mb_tag {
	uint32_t	type;
	uint32_t	size;
};

struct mb_mmap_entry {
	uint64_t	addr;
	uint64_t	len;
	uint32_t	type;
	uint32_t	zero;
} __attribute__((packed));

struct mb_tag_mmap {
	uint32_t				type;
	uint32_t				size;
	uint32_t				ent_size;
	uint32_t				ent_version;
	struct mb_mmap_entry	entries[0];
};

#endif
