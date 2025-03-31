#ifndef VMM_H
#define VMM_H

#include "klib.h"

#define KERNEL_START 0xC0000000

#define PAGE_SIZE	4096
#define ENT_SIZE	1024

#define VMM_ENT_PRESENT		(1 << 0) // Entry is present
#define VMM_ENT_WRITE		(1 << 1) // Entry is writeable
#define VMM_ENT_USER		(1 << 2) // Entry is accessible to user mode
#define VMM_ENT_ACCESSED	(1 << 5) // Entry has been accessed
#define VMM_ENT_DIRTY		(1 << 6) // Entry has been written to

#define PDIR_IDX(a) (a >> 22)
#define PTBL_IDX(a) ((a >> 12) & 0x3FF)

void	vmm_init(void);

#endif // VMM_H
