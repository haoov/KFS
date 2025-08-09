#include "memblock.h"
#include "multiboot2.h"
#include "intf.h"

static struct memblock_manager mb_manager;

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
}
