#include "vmm.h"

extern uint32_t	kernel_start;
extern uint32_t	page_dir[1024];

void	invalidate_page(uint32_t vaddr)
{
	__asm__ volatile("invlpg %0" :: "m"(vaddr));
}

void	invalidate_page_table(uint32_t pdi) {
	uint32_t vaddr;

	for (uint32_t pti = 0; pti < 1024; ++pti) {
		vaddr = pdi << 22 | pti << 12;
		invalidate_page(vaddr);
	}
}

void	vmm_init(void)
{
	// Recursive mapping for page_dir allows to access using virtual addresses
	page_dir[1023] = (uint32_t)page_dir - (uint32_t)&kernel_start;
	page_dir[1023] |= VMM_ENT_WRITE | VMM_ENT_PRESENT;

	// Unmapping the first entry and updating the TLB
	page_dir[0] = 0;
	invalidate_page_table(0);

}
