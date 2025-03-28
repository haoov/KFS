#ifndef PAGING_H
#define PAGING_H

#include "klib.h"

#define PAGE_SIZE	4096

#define ENT_PRESENT	0x01 // Entry is present
#define ENT_WRITE		0x02 // Entry is writeable
#define ENT_USER		0x04 // Entry is accessible to user mode
#define ENT_ACCESSED	0x20 // Entry has been accessed
#define ENT_DIRTY		0x40 // Entry has been written to

#define PDIR_IDX(a) (a >> 22)
#define PTBL_IDX(a) ((a >> 12) & 0x3FF)

void	init_paging(void);

#endif // PAGING_H
