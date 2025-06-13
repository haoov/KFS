#include "memory.h"
#include "multiboot2.h"
#include "intf.h"

int count_regions(struct mb_tag *start, struct mb_tag *end) {
	struct mb_tag *tag = start;
	int mmap_entries = 0;

	while (tag < end && tag->type) {
		if (tag->type == MB_TAG_TYPE_MMAP) {
			struct mb_tag_mmap *mmap_tag = (struct mb_tag_mmap*)tag;

			uint32_t entries_size = mmap_tag->size - sizeof(struct mb_tag_mmap);
			mmap_entries = entries_size / mmap_tag->ent_size;

			return mmap_entries;
		}

		tag = (struct mb_tag*)((uint8_t*)tag + ALIGN(tag->size, 8));
	}

	return 0;
}

void memblock_init(uint64_t mb_infos_addr) {
	uint32_t *mb_infos = (uint32_t*)mb_infos_addr;
	uint32_t total_size = mb_infos[0];

	struct mb_tag *start = (struct mb_tag *)(mb_infos_addr + 8);
	struct mb_tag *end = (struct mb_tag *)(mb_infos_addr + total_size);

	int region_nb = count_regions(start, end);

}
