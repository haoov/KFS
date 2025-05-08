#include "kmem.h"
#include "vmm.h"
#include "pmm.h"
#include "intf.h"

kmem_cache_t	*cache_chain = NULL;
kmem_cache_t	*free_cache = NULL;

/* Allocates a page (4096 bytes) of physical memory and map it
 *
 * return: virtual address of mapped page
 * */
void	*kmem_alloc_page(void) {
	void	*paddr;
	void	*vaddr;

	paddr = pmm_alloc_pages(1);
	if (!paddr) return NULL;

	vaddr = vmm_map_page(paddr, VMM_ENT_WRITE);
	if (!vaddr) {
		pmm_free_pages(paddr, 1);
		return NULL;
	}

	return vaddr;
}

void	init_chache_chain(void) {
	kmem_cache_t	*cache;
	uint32_t		next, end, size;

	cache_chain = kmem_alloc_page();
	if (!cache_chain) return;

	cache = cache_chain;
	size = sizeof(kmem_cache_t);
	next = (uint32_t)cache_chain + size;
	end = (uint32_t)cache_chain + PAGE_SIZE;
	while (next + size <= end) {
		cache->next = (kmem_cache_t*)next;
		cache = (kmem_cache_t*)next;
		next += size;
	}
	cache->next = NULL;

	cache = cache_chain;

	free_cache = cache_chain;
}

kmem_slab_t	*create_slab(kmem_cache_t *cache) {
	kmem_slab_t	*slab;

	slab = kmem_alloc_page();
	if (!slab) return NULL;

	return slab;
}

kmem_cache_t	*create_cache(uint32_t obj_size) {
	kmem_cache_t	*cache;

	if (!free_cache) return NULL;

	cache = free_cache;

	cache->obj_size = obj_size;
	cache->slabs_full = NULL;
	cache->slabs_partial = NULL;
	cache->page_per_slab = 1;
	cache->slabs_free = create_slab(cache);
	if (cache->slabs_free) return NULL;

	free_cache = free_cache->next;

	return cache;
}

void	kmem_init(void) {
	init_chache_chain();
	kmem_cache_t *cache = cache_chain;

	while (cache) {
		kprint("cache address: %x\n", cache);
		cache = cache->next;
	}
}
