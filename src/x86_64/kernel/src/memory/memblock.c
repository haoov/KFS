#include "memblock.h"
#include "multiboot2.h"
#include "intf.h"

// Initiliaze static structures
static struct memblock_region mb_memory_init[INIT_MEMEBLOCK_REGIONS];
static struct memblock_region mb_reserved_init[INIT_MEMEBLOCK_REGIONS];
static struct memblock_manager mb_manager = {
	.memory = {
		.count = 0,
		.max = INIT_MEMEBLOCK_REGIONS,
		.total_size = 0,
		.regions = mb_memory_init
	},
	.reserved = {
		.count = 0,
		.max = INIT_MEMEBLOCK_REGIONS,
		.total_size = 0,
		.regions = mb_reserved_init
	}
};

/* Add the specified memory range to memblock type
 * */
void memblock_add(struct memblock_type *type, uint64_t addr, uint64_t size) {
	if (type->count == INIT_MEMEBLOCK_REGIONS) {
		// Resize
		return;
	}
	struct memblock_region *region = &type->regions[type->count];
	++type->count;
	region->base = addr;
	region->size = size;
	type->total_size += size;
}

/* Remove memory region from reserved ones
 * */
int memblock_free(uint64_t base, uint64_t size) {
	struct memblock_type *reserved = &mb_manager.reserved;

	for (uint64_t count = 0; count < reserved->count; ++count) {
		struct memblock_region *region = &reserved->regions[count];

		if (region->base == base && region->size == size) {
			for (uint64_t i = count; i < reserved->count - 1 ; ++i) {
				reserved->regions[i] = reserved->regions[i + 1];
			}
			--reserved->count;
			reserved->total_size -= size;
			return 0;
		}
	}
	return -1;
}

/* Check if a given memory range is reserved
 * */
bool is_reserved(uint64_t base, uint64_t size) {
	struct memblock_type *reserved = &mb_manager.reserved;
	
	for (uint64_t count = 0; count < reserved->count; ++count) {
		struct memblock_region *region = &reserved->regions[count];
		uint64_t region_end = region->base + region->size;
		uint64_t end = base + size;

		if (base <= region_end && end >= region->base) {
			return true;
		}
	}
	return false;
}

void *memblock_alloc(uint64_t size) {
	int align = sizeof(void*);
	struct memblock_type *memory = &mb_manager.memory;

	// Search for memory region
	for (uint64_t count = 0; count < memory->count; ++count) {
		struct memblock_region *region = &memory->regions[count];

		uint64_t start = ALIGN(region->base, align);
		uint64_t end = region->base + region->size;

		if (start + size > end) {
			continue;
		}

		// Search for space in region
		for (uint64_t addr = start; addr < end - size; addr += align) {
			if (!is_reserved(addr, size)) {
				memblock_add(&mb_manager.reserved, addr, size);
				return (void*)addr;
			}
		}
	}
	return NULL;
}

/* Parse multiboot structure to get available memory regions
 * */
void parse_mmap(struct mb_tag *start, struct mb_tag *end) {
	struct mb_tag *tag = start;
	int mmap_entries = 0;

	while (tag < end && tag->type) {
		if (tag->type == MB_TAG_TYPE_MMAP) {
			// Tag represent memory map entries
			struct mb_tag_mmap *mmap_tag = (struct mb_tag_mmap*)tag;

			// Get number of memory map entries
			uint32_t entries_size = mmap_tag->size - sizeof(struct mb_tag_mmap);
			mmap_entries = entries_size / mmap_tag->ent_size;

			for (int i = 0; i < mmap_entries; ++i) {
				struct mb_mmap_entry ent = mmap_tag->entries[i];

				if (ent.type == MB_MEMORY_AVAILABLE) {
					// Address range is available so add it to the manager
					memblock_add(&mb_manager.memory, ent.addr, ent.len);
				}
			}

		}

		tag = (struct mb_tag*)((uint8_t*)tag + ALIGN(tag->size, 8));
	}
}

void memblock_init(uint64_t mb_infos_addr) {
	uint32_t *mb_infos = (uint32_t*)mb_infos_addr;
	uint32_t total_size = mb_infos[0];

	struct mb_tag *start = (struct mb_tag *)(mb_infos_addr + 8);
	struct mb_tag *end = (struct mb_tag *)(mb_infos_addr + total_size);

	parse_mmap(start, end);
	memblock_add(&mb_manager.reserved, 0, 0xFFFFF);
}
