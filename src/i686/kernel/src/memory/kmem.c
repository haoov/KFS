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

void kmem_free_page(void *addr) {
	uint32_t *pte;
	void *paddr;

	// Get page table entry
	pte = PTE_VADDR(PDIR_IDX(addr), PTAB_IDX(addr));
	// Removing flags to get the physical address
	paddr = (void*)(*pte & 0xFFFFF000);

	pmm_free_pages(paddr, 1);
	vmm_umap_page(addr);
}

/* Allocate and set cache chain
 * */
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

	free_cache = cache_chain;
}

kmem_slab_t	*create_slab(kmem_cache_t *cache) {
	kmem_slab_t	*slab = NULL;
	slab_obj_t *obj = NULL;
	uint32_t meta_size = sizeof(kmem_slab_t);

	slab = kmem_alloc_page();
	if (!slab) return NULL;

	slab->s_mem = (void*)((uint32_t)slab + meta_size);
	slab->total_obj = (PAGE_SIZE - meta_size - 1) / cache->obj_size;
	slab->free_obj = slab->total_obj;
	slab->free_list = (slab_obj_t*)slab->s_mem;
	slab->next = NULL;

	obj = slab->free_list;
	for (int i = 0; i < slab->total_obj; ++i) {
		obj->next_free = (slab_obj_t*)((uint32_t)obj + cache->obj_size);
		obj = obj->next_free;
	}
	obj->next_free = NULL;

	return slab;
}

kmem_slab_t *add_slab(kmem_cache_t *cache) {
	kmem_slab_t *slab = NULL;
	kmem_slab_t *last = NULL;

	slab = create_slab(cache);
	if (!slab) return NULL;
	
	if (!cache->slabs) {
		cache->slabs = slab;
	}
	else {
		last = cache->slabs;
		while (last->next) {
			last = last->next;
		}
		last->next = slab;
	}

	return slab;
}

void remove_slab(kmem_slab_t *slab, kmem_cache_t *cache) {
	kmem_slab_t *cur, *prev;
	void *paddr;

	cur = cache->slabs;
	if (cur == slab) {
		cache->slabs = cur->next;
	}
	else {
		while (cur && cur != slab) {
			prev = cur;
			cur = cur->next;
		}
		if (!cur) return;
		prev->next = cur->next;
	}
	kmem_free_page(slab);
}

kmem_cache_t	*create_cache(uint32_t obj_size) {
	kmem_cache_t	*cache;

	if (!free_cache) return NULL;

	cache = free_cache;

	cache->obj_size = obj_size;
	cache->page_per_slab = 1;
	cache->slabs = NULL;

	free_cache = free_cache->next;

	return cache;
}

kmem_cache_t *get_cache(uint32_t size) {
	kmem_cache_t *cache = cache_chain;

	while (cache) {
		if (cache->obj_size >= size) {
			return cache;
		}
		cache = cache->next;
	}

	return NULL;
}

/* Get slab with free object from cache or add one
 * */
kmem_slab_t *get_slab(kmem_cache_t *cache) {
	kmem_slab_t *slab;

	slab = cache->slabs;
	while (slab) {
		if (slab->free_obj) {
			return slab;
		}
		slab = slab->next;
	}

	slab = add_slab(cache);

	return slab;
}

/* Get first free object in slab of requested size cache
 * */
void	*kmalloc(uint32_t size) {
	kmem_cache_t *cache;
	kmem_slab_t *slab;
	void		*addr;

	kprint("\nALLOCATING MEMORY (%d bytes)\n", size);
	cache = get_cache(size);
	if (!cache) return NULL;

	slab = get_slab(cache);
	if (!slab) return NULL;

	kprint("cache: %x, object size: %d, slab: %x\n", cache, cache->obj_size, slab);
	// Get the object from the slab's free list and update it
	addr = slab->free_list;
	slab->free_list = slab->free_list->next_free;
	--slab->free_obj;

	// Set object to 0
	memset(addr, 0, cache->obj_size);
	kprint("memory allocated at: %x\n", addr);

	return addr;
}

/* Search in cache for slab containing a given address
 * */
kmem_slab_t *find_slab(kmem_cache_t *cache, void *addr) {
	kmem_slab_t *slab = NULL;
	uint32_t slab_size;

	slab = cache->slabs;
	while (slab) {
		slab_size = slab->total_obj * cache->obj_size;
		if (addr >= slab->s_mem && addr <= slab->s_mem + slab_size) {
			return slab;
		}
		slab = slab->next;
	}

	return slab;
}

/* Set given address as free object
 * */
void kfree(void *addr) {
	kprint("\nFREEING MEMORY: %x\n", addr);
	kmem_cache_t *cache;
	kmem_slab_t *slab = NULL;
	slab_obj_t *obj;

	// Check if address is valid
	if (!addr) return;

	// Find slab containing address
	cache = cache_chain;
	while (cache) {
		slab = find_slab(cache, addr);
		if (slab) break;
		cache = cache->next;
	}
	if (!cache) return;

	kprint("cache: %x, slab: %x\n", cache, slab);

	// Check if address is not already on free list
	obj = slab->free_list;
	while (obj) {
		if (obj == addr) {
			return;
		}
		obj = obj->next_free;
	}

	// Add address to free list
	obj = (slab_obj_t*)addr;
	obj->next_free = slab->free_list;
	slab->free_list = obj;
	++slab->free_obj;

	if (slab->free_obj == slab->total_obj) {
		remove_slab(slab, cache);
	}
}

/* Return the size in memory of the variable at address <addr>
 * */
uint32_t kmem_get_size(void *addr) {
	kmem_cache_t *cache;
	kmem_slab_t *slab;

	// Find slab containing address
	cache = cache_chain;
	while (cache) {
		slab = find_slab(cache, addr);
		if (slab) break;
		cache = cache->next;
	}
	if (!cache) return 0;
	
	return cache->obj_size;
}

void	kmem_init(void) {
	init_chache_chain();
	create_cache(4);
	create_cache(8);
	create_cache(16);
	create_cache(32);
	create_cache(64);
	create_cache(128);
	create_cache(256);
	create_cache(512);
	create_cache(1024);
	create_cache(2048);
}
