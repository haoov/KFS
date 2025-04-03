#include "vmm.h"
#include "pmm.h"
#include "intf.h"

extern uint32_t	page_dir[1024];
uint32_t		pt_alloc_bitmap[ENT_SIZE / 32];
uint32_t		*page_tables;

void	set_alloc_bit(uint32_t bit) {
	pt_alloc_bitmap[bit / 32] |= (1 << (bit % 32));
}

void	clear_alloc_bit(uint32_t bit) {
	pt_alloc_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

bool	test_alloc_bit(uint32_t bit) {
	return 1 & (pt_alloc_bitmap[bit / 32] >> (bit % 32));
}

void	invalidate_page(uint32_t vaddr)
{
	__asm__ volatile("invlpg %0" :: "m"(vaddr));
}

void	invalidate_pde(uint32_t pdi) {
	uint32_t vaddr;

	for (uint32_t pti = 0; pti < 1024; ++pti) {
		vaddr = pdi << 22 | pti << 12;
		invalidate_page(vaddr);
	}
}

void	vmm_init(void) {
	// Recursive mapping for page_dir allows to access using virtual addresses
	page_dir[1023] = (uint32_t)page_dir - KERNEL_VIRT_BASE_ADDR;
	page_dir[1023] += VMM_ENT_WRITE | VMM_ENT_PRESENT;

	// Unmapping the first entry and updating the TLB
	page_dir[0] = 0;
	invalidate_pde(0);

	// Init page table allocation bitmap
	for (uint32_t i = 0; i < ENT_SIZE; ++i) {
		if (i == 768) {
			set_alloc_bit(i);
			continue;
		}

		clear_alloc_bit(i);
	}
}

uint32_t	new_page_table(uint32_t pdi, uint32_t flags) {
	uint32_t pt;
	uint32_t *pde;
	uint32_t *pte;

	pde = PDE_VADDR(pdi);

	// We need 4KB in memory so one page for a page table (1024 * 4)
	pt = (uint32_t)pmm_alloc_pages(1);
	if (!pt) {
		return 0;
	}

	set_alloc_bit(pdi);

	// Set the page directoy entry
	*pde = pt | VMM_ENT_PRESENT | flags;

	// Init all page table entries to 0
	pte = PTE_VADDR(pdi, 0);
	for (uint32_t i = 0; i < ENT_SIZE; ++i) {
		pte[i] = 0;
	}

	return *pde;
}

void	vmm_umap_page(void *vaddr) {
	uint32_t *pte;

	pte = PTE_VADDR(PDIR_IDX(vaddr), PTAB_IDX(vaddr));
	*pte = 0;
	invalidate_page((uint32_t)vaddr);
}

void	*vmm_map_page(void *addr, uint32_t flags) {
	uint32_t *pde;
	uint32_t *pte;

	for (uint32_t pdi = 0; pdi < ENT_SIZE; ++pdi) {
		pde = PDE_VADDR(pdi);

		// Allocate a new page table if there is none
		if (!test_alloc_bit(pdi)) {

			*pde = new_page_table(pdi, VMM_ENT_WRITE);
			if (!*pde) break;

		}
		for (uint32_t pti = 0; pti < ENT_SIZE; ++pti) {
			pte = PTE_VADDR(pdi, pti);

			// Leave the first page empty for NULL pointer (addr 0x00000000)
			if (!pdi && !pti) continue;

			// If page table entry is empty map it and return virtual address
			if (!*pte) {
				*pte = (uint32_t)addr | VMM_ENT_PRESENT | flags;
				return VADDR(pdi, pti);
			}
		}
	}

	return NULL;
}

