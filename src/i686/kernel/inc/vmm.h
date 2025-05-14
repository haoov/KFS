#ifndef VMM_H
#define VMM_H

#include "klib.h"

#define KERNEL_VIRT_BASE_ADDR 0xC0000000

#define PAGE_SIZE	4096
#define ENT_SIZE	1024

#define VMM_ENT_PRESENT		(1 << 0) // Entry is present
#define VMM_ENT_WRITE		(1 << 1) // Entry is writeable
#define VMM_ENT_USER		(1 << 2) // Entry is only user mode
#define VMM_ENT_ACCESSED	(1 << 5) // Entry has been accessed
#define VMM_ENT_DIRTY		(1 << 6) // Entry has been written to

#define PDIR_IDX(a) ((uint32_t)a >> 22)
#define PTAB_IDX(a) (((uint32_t)a >> 12) & 0x3FF)
#define POFF(a) ((uint32_t)a & 0xFFF)
#define VADDR(di, ti) (void*)(di << 22 | ti << 12)

#define PD_VADDR 0xFFFFF000
#define FPT_VADDR 0xFFC00000

#define PDE_VADDR(i) (void*)(PD_VADDR + (i * 4))
#define PTE_VADDR(di, ti) (void*)(FPT_VADDR + (di * 0x1000) + (ti * 4))

void	vmm_init(void);
void	*vmm_map_page(void *paddr, uint32_t flags);
void	vmm_umap_page(void *vaddr);

#endif // VMM_H
