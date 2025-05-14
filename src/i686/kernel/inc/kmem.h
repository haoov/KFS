#ifndef KMEM_H
#define KMEM_H

#include "klib.h"

typedef struct slab_obj {
	struct slab_obj *next_free;
} slab_obj_t;

typedef struct kmem_slab {
	void				*s_mem;			// Starting address of first object
	uint32_t			total_obj;	// Total number of ojbects in slab
	uint32_t			free_obj;		// Number of free objects in slab
	slab_obj_t			*free_list;
	struct kmem_slab	*next;
} kmem_slab_t;

typedef struct kmem_cache {
	uint32_t			obj_size;
	uint32_t			page_per_slab;
	kmem_slab_t			*slabs;
	struct kmem_cache	*next;
} kmem_cache_t;

void kmem_init(void);
void *kmalloc(uint32_t size);
void kfree(void *addr);
uint32_t kmem_get_size(void *addr);

#endif
