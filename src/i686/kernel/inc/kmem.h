#ifndef KMEM_H
#define KMEM_H

#include "klib.h"

typedef struct kmem_slab {
	void				*s_mem; // Stating address of first object
	uint32_t			inuse;
	uint32_t			free; // Array of location for free objects
	struct kmem_slab	*next;
} kmem_slab_t;

typedef struct kmem_cache {
	uint32_t			obj_size;
	uint32_t			page_per_slab;
	kmem_slab_t			*slabs_free;
	kmem_slab_t			*slabs_partial;
	kmem_slab_t			*slabs_full;
	struct kmem_cache	*next;
} kmem_cache_t;

void	kmem_init(void);

#endif
