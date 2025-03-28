#include "paging.h"

extern uint32_t	kernel_start;

static uint32_t	pdir[1024] __attribute__((aligned(4096)));
static uint32_t	ptbls[1024][1024] __attribute__((aligned(4096)));

void	identity_map(void)
{
	uint32_t	pent;

	pdir[0] = (uint32_t)ptbls[0] | ENT_WRITE | ENT_PRESENT;

	// Identity mapping for first 4KB
	for (uint32_t i = 0; i < 1024; ++i)
	{
		pent = i * PAGE_SIZE | ENT_WRITE | ENT_PRESENT;

		ptbls[0][i] = pent;
	}
}

void	higher_half_map(void)
{
	// Map the kernel to the higher half virtual addresses
	pdir[768] = pdir[0];

	for (uint32_t i = 0; i < 1024; ++i)
	{
		ptbls[768][i] = ptbls[0][i];
	}
}

void	map_page(uint32_t paddr, uint32_t vaddr, uint32_t flags)
{
	uint32_t	pdi = PDIR_IDX(vaddr);

	if (!(pdir[pdi] & ENT_PRESENT))
	{
		memset(ptbls[pdi], 0, sizeof(uint32_t) * 1024);
		pdir[pdi] = (uint32_t)ptbls[pdi];
		pdir[pdi] |= ENT_WRITE | ENT_PRESENT | (flags & ENT_USER);
	}
	ptbls[pdi][PTBL_IDX(paddr)] = paddr | flags | ENT_PRESENT;
}

void	umap_page(uint32_t vaddr)
{
	uint32_t	pdi = PDIR_IDX(vaddr);

	if ((pdir[pdi] & ENT_PRESENT))
	{
		ptbls[pdi][PTBL_IDX(vaddr)] = 0;
	}
}

void	init_paging(void)
{
	memset(pdir, 0, sizeof(uint32_t) * 1024);
	memset(ptbls[0], 0, sizeof(uint32_t) * 1024);
	identity_map();
	higher_half_map();
}
