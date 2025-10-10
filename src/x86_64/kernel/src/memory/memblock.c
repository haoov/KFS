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

void memblock_add(uint64_t addr, uint64_t size) {
	if (mb_manager.memory.count + 1 == mb_manager.memory.max) {
		// Resize
		return;
	}
	++mb_manager.memory.count;
	struct memblock_region *region = &mb_manager.memory.regions[mb_manager.memory.count];
	region->base = addr;
	region->size = size;
}

void count_regions(struct mb_tag *start, struct mb_tag *end) {
	struct mb_tag *tag = start;
	int mmap_entries = 0;

	while (tag < end && tag->type) {
		if (tag->type == MB_TAG_TYPE_MMAP) {
			struct mb_tag_mmap *mmap_tag = (struct mb_tag_mmap*)tag;

			uint32_t entries_size = mmap_tag->size - sizeof(struct mb_tag_mmap);
			mmap_entries = entries_size / mmap_tag->ent_size;

			for (int i = 0; i < mmap_entries; ++i) {
				struct mb_mmap_entry ent = mmap_tag->entries[i];

				if (ent.type == MB_MEMORY_AVAILABLE) {
					++mb_manager.memory.count;
					mb_manager.memory.total_size += ent.len;
				}
				else if (ent.type == MB_MEMORY_RESERVED) {
					++mb_manager.reserved.count;
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


	count_regions(start, end);
	kprint_str("Available memory regions: ");
	kprint_int(mb_manager.memory.count);
	kprint_char('\n');
	kprint_str("Reserved memory regions: ");
	kprint_int(mb_manager.reserved.count);
	kprint_char('\n');
}
