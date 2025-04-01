#ifndef PMM_H
#define PMM_H

#include "klib.h"
#include "boot.h"

void	pmm_init(mmap_ent_t *mmap, uint32_t mmap_len);
void	*pmm_alloc_pages(uint32_t n);
void	pmm_free_pages(void *addr, uint32_t n);

#define ALIGN(v, n)	((v + (n - 1)) & ~(n - 1))
#define NEXT_MMAP_ENT_ADDR(e) ((uint32_t)e + e->size + sizeof(e->size))
#define ADDR_TO_PG_IDX(a) (a / PAGE_SIZE)
#define PG_IDX_TO_ADDR(i) ((void*)(i * PAGE_SIZE))

#endif
