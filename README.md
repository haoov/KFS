# KFS
Kernel From Scratch project from 42 School

## Paging
Paging is a system which translate a **virtual address** into the coresponding **physical address**. Using this system, the kernel can manage memory with page-level protection and memory isolation for each processes.<br>
On a 32bit system, paging is divided into the **page directory** and the **page tables**.<br>
This allows processes to access the full 4GB memory space of 32bit systems.
### Page directory
The page directory has 1024 entries pointing to the page tables:
|31 ... 12|11 ... 8|7|6|5|4|3|2|1|
| ------- | ------ |-|-|-|-|-|-|-|
|bits 31 to 12 of page table's physical address | AVL |PS(0)|AVL|A|PCD|PWT|U/S|R/W|P|

### Page table
Each of the page tables has also 1024 entries pointing to physical 4KB pages
|31 ... 12|11 ... 9|8|7|6|5|4|3|2|1|
| ------- | ------ |-|-|-|-|-|-|-|-|
|bits 31 to 12 of 4KB memory frame's physical address | AVL |G|PAT|D|A|PCD|PWT|U/S|R/W|P|

Due to the way page directory and page tables entries are made, they MUST be aligned on 4096 boundaries so that the first 12 bits of the addresses are 0.
### Virtual address
A virtual address is made of the following
|31 ... 22|21 ... 12|11 ... 0|
|---------|---------|--------|
|Index into the page directoy|Index into the page table| offset into the memory frame|

### Enable paging
To tell the kernel that we will be accessing memory with virtual addresses using the page directoty and the page tables we need to set the 
