#include "vmm.h"
#include "pmm.h"
#include "intf.h"

extern uint32_t	page_dir[1024];
uint32_t		pt_alloc_bitmap[ENT_SIZE / 32];
uint32_t		*page_tables;

static vma_region_t vma_pool[VMA_MAX_ENTRIES];
static uint32_t vma_pool_index = 0;
static vma_region_t *vma_free_list = NULL;
static vma_region_t *vma_reserved_list = NULL;

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
	__asm__ volatile("invlpg (%0)" :: "r"(vaddr) : "memory");
}

void	invalidate_pde(uint32_t pdi) {
	uint32_t vaddr;

	for (uint32_t pti = 0; pti < 1024; ++pti) {
		vaddr = (uint32_t)VADDR(pdi, pti);
		invalidate_page(vaddr);
	}
}

void	vmm_init(void) {
	// Recursive mapping
	page_dir[1023] = (uint32_t)page_dir - KERNEL_VIRT_BASE_ADDR;
	page_dir[1023] += VMM_ENT_WRITE | VMM_ENT_PRESENT;

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

	memset(vma_pool, 0, sizeof(vma_pool));
	vma_pool_index = 0;
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

	kprint("New page table allocated\n");
	return *pde;
}

vma_region_t *vma_find_region(uint32_t vaddr) {
	for (vma_region_t *region = vma_reserved_list; region; region = region->next) {
		if (vaddr >= region->start && vaddr < region->end) {
			return region;
		}
	}
	return NULL;
}

vma_region_t *vma_alloc_region(void) {
	vma_region_t *region;

	if (vma_free_list) {
		region = vma_free_list;
		vma_free_list = vma_free_list->next;
		memset(region, 0, sizeof(vma_region_t));
		return region;
	}

	if (vma_pool_index == VMA_MAX_ENTRIES) {
		return NULL;
	}

	region = &vma_pool[vma_pool_index++];
	region->next = vma_reserved_list;
	vma_reserved_list = region;

	return region;
}

void vma_free_region(vma_region_t *region) {
	region->next = vma_free_list;
	vma_free_list = region;
}

vma_region_t *vma_new_region(uint32_t start, uint32_t flags, uint32_t pg_nb) {
	vma_region_t *region = vma_alloc_region();

	if (!region) {
		return NULL;
	}

	region->start = start;
	region->end = start + (pg_nb << 12);
	region->flags = flags;
	
	return (region);
}

void *vmm_reserve_range(uint32_t size, uint32_t flags) {
	uint32_t page_needed = ALIGN(size, PAGE_SIZE) >> 12;

	for (uint32_t pdi = 0; pdi < ENT_SIZE; ++pdi) {
		for (uint32_t pti = 1; pti < ENT_SIZE; ++pti) {
			bool free_range = true;

			for (uint32_t i = 0; i < page_needed; ++i) {
				uint32_t check_pdi = pdi + ((pti + i) / ENT_SIZE);
				uint32_t check_pti = (pti + i) % ENT_SIZE;

				if (check_pdi > ENT_SIZE) {
					free_range = false;
					break;
				}

				uint32_t vaddr = (uint32_t)VADDR(check_pdi, check_pti);
				if (vma_find_region(vaddr)) {
					free_range = false;
					break;
				}
			}

			if (free_range) {
				uint32_t start = (uint32_t)VADDR(pdi, pti);
				vma_new_region(start, flags, page_needed);
				return (void*)start;
			}
		}
	}
	return NULL;
}

// Kernel tried to access memory not allocated
bool vmm_handle_page_fault(uint32_t fault_addr, uint32_t err_code) {
	// Check if region is reserved
	vma_region_t *region = vma_find_region(fault_addr);

	if (!region) {
		return false; // Invalid access
	}

	// TODO: More checks like protection and access rights

	uint32_t pdi = PDIR_IDX(fault_addr);
	uint32_t pti = PTAB_IDX(fault_addr);
	uint32_t *pde = PDE_VADDR(pdi);
	uint32_t *pte;

	// Allocate page table if needed
	if (!test_alloc_bit(pdi)) {
		if (!new_page_table(pdi, VMM_ENT_WRITE)) {
			return false;
		}
	}

	// Allocate page
	uint32_t page_paddr = (uint32_t)pmm_alloc_pages(1);
	if (!page_paddr) {
		return false; // Out of memory
	}

	// Map the page
	pte = PTE_VADDR(pdi, pti);
	*pte = page_paddr | VMM_ENT_PRESENT | region->flags;

	invalidate_page(fault_addr & 0xFFFFF000);

	return true;
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

