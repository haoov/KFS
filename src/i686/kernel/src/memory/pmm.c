#include "pmm.h"
#include "boot.h"
#include "vmm.h"
#include "intf.h"
#include "intf.h"

uint32_t	*bitmap = NULL;
uint32_t	total_pages = 0;
uint32_t	free_pages = 0;

void	pmm_init(mmap_ent_t *mmap, uint32_t mmap_len) {
	mmap_ent_t	*ent;
	uint32_t	highest_address = 0;
	uint32_t	tmp_addr = 0;

	ent = mmap;
	while ((uint32_t)ent < (uint32_t)mmap + mmap_len) {
		if (ent->type == MMAP_TYPE_AVAILABE) {
			tmp_addr = ent->base_addr_low + ent->lenght_low;
			if (highest_address < tmp_addr) {
				highest_address = tmp_addr;
			}
		}
		ent = (mmap_ent_t *)((uint32_t)ent + ent->size + sizeof(ent->size));
	}

	total_pages = highest_address / PAGE_SIZE;
	kprint("Total pages: %d\n", total_pages);
}
