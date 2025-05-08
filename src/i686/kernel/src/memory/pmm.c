#include "pmm.h"
#include "boot.h"
#include "vmm.h"
#include "intf.h"

extern uint32_t	kernel_start;
extern uint32_t	kernel_end;
uint32_t		*bitmap = NULL;
uint32_t		total_pages = 0;
uint32_t		free_pages = 0;
uint32_t		addr_min = 0;
uint32_t		addr_max = 0;

void	set_bit(uint32_t bit) {
	bitmap[bit / 32] |= (1 << (bit % 32));
}

void	clear_bit(uint32_t bit) {
	bitmap[bit / 32] &= ~(1 << (bit % 32));
}

bool	test_bit(uint32_t bit) {
	return 1 & (bitmap[bit / 32] >> (bit % 32));
}

uint32_t get_free_pages(uint32_t n) {
	uint32_t	count;

	count = 0;
	for (uint32_t i = 0; i < total_pages; ++i) {
		while (!test_bit(i)) {
			++count;
			++i;
			if (count == n) {
				return i - count;
			}
		}
		count = 0;
	}
	return (uint32_t)-1;
}

void	*pmm_alloc_pages(uint32_t n) {
	uint32_t	pg_idx;

	if (!free_pages) {
		return NULL;
	}

	pg_idx = get_free_pages(n);
	if (pg_idx == (uint32_t)-1) {
		return NULL;
	}
	for (uint32_t pg = pg_idx; pg < pg_idx + n; ++pg) {
		set_bit(pg);
		--free_pages;
	}

	return PG_IDX_TO_ADDR(pg_idx);
}

void	pmm_free_pages(void *addr, uint32_t n) {
	uint32_t	pg_idx;

	pg_idx = ADDR_TO_PG_IDX((uint32_t)addr);
	if (pg_idx + n > total_pages) {
		return;
	}

	for (uint32_t pg = pg_idx; pg < pg_idx + n; ++pg) {
		if (test_bit(pg)) {
			clear_bit(pg);
			++free_pages;
		}
	}

}

void	bitmap_init(mmap_ent_t *mmap, uint32_t mmap_len) {
	mmap_ent_t	*ent = NULL;
	uint32_t	bm_len, bm_size, pkernel_end, pkernel_start;

	// First find suitable location for bitmap
	// Get kernel end physical address
	pkernel_end = (uint32_t)&kernel_end - KERNEL_VIRT_BASE_ADDR;
	// Compute the size of the bitmap
	bm_len = (total_pages / 32) + (total_pages % 32 ? 1 : 0);
	bm_size = bm_len * sizeof(uint32_t);

	ent = mmap;
	while ((uint32_t)ent < (uint32_t)mmap + mmap_len) {
		if (ent->type == MMAP_TYPE_AVAILABLE) {
			uint32_t base, len, end;

			base = ent->base_addr_low;
			len = ent->lenght_low;
			end = base + len;

			// Find space just after kenel
			if (base <= pkernel_end && pkernel_end < end) {
				uint32_t bm_loc;

				bm_loc = ALIGN(pkernel_end, PAGE_SIZE);

				if (bm_loc + bm_size <= end) {
					bitmap = (uint32_t*)bm_loc;
					break;
				}
			}
		}
		ent = (mmap_ent_t *)NEXT_MMAP_ENT_ADDR(ent);
	}

	// Set all blocks as used
	for (uint32_t i = 0; i < bm_len; ++i) {
		bitmap[i] = 0xFFFFFFFF;
	}

	// Then clear all available memory in the bitmap
	ent = mmap;
	while ((uint32_t)ent < (uint32_t)mmap + mmap_len) {
		if (ent->type == MMAP_TYPE_AVAILABLE) {
			uint32_t pg_start, pg_end, base, len, end;

			base = ent->base_addr_low;
			len = ent->lenght_low;
			end = base + len;
			pg_start = CEIL_DIV(base, PAGE_SIZE);
			pg_end = ADDR_TO_PG_IDX(end);

			for (uint32_t pg = pg_start; pg < pg_end; ++pg) {
				clear_bit(pg);
				++free_pages;
			}
		}
		ent = (mmap_ent_t *)NEXT_MMAP_ENT_ADDR(ent);
	}

	// Finally set bitmap and kernel as used
	pkernel_start = (uint32_t)&kernel_start - KERNEL_VIRT_BASE_ADDR;
	uint32_t kernel_pg_end, bitmap_pg_start, bitmap_pg_end;

	kernel_pg_end = CEIL_DIV(pkernel_end, PAGE_SIZE);
	bitmap_pg_start = ADDR_TO_PG_IDX((uint32_t)bitmap);
	bitmap_pg_end = CEIL_DIV((uint32_t)bitmap + bm_size, PAGE_SIZE);

	// Set from address 0 to the kernel as used
	for (uint32_t pg = 0; pg < kernel_pg_end; ++pg) {
		set_bit(pg);
		--free_pages;
	}
	for (uint32_t pg = bitmap_pg_start; pg < bitmap_pg_end; ++pg) {
		set_bit(pg);
		--free_pages;
	}

}

void	pmm_init(mmap_ent_t *mmap, uint32_t mmap_len) {
	mmap_ent_t	*ent = NULL;
	uint32_t	tmp_addr = 0;

	ent = mmap;
	addr_min = ent->base_addr_low;
	while ((uint32_t)ent < (uint32_t)mmap + mmap_len) {
		if (ent->type == MMAP_TYPE_AVAILABLE) {
			tmp_addr = ent->base_addr_low + ent->lenght_low;
			if (addr_max < tmp_addr) {
				addr_max = tmp_addr;
			}
			if (addr_min > ent->base_addr_low) {
				addr_min = ent->base_addr_low;
			}
		}
		ent = (mmap_ent_t *)NEXT_MMAP_ENT_ADDR(ent);
	}

	total_pages = addr_max / PAGE_SIZE;
	bitmap_init(mmap, mmap_len);
}
