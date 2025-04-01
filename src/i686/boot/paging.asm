; Paging boot process, declare the page directory and one page table for
; identity mapping. Map the kernel to higher half address 0xC0000000,
; identity maps the first 4MB of memory, enable paging then return to calling
; function. For more details see README#paging

KERNEL_START EQU 0xC0000000
PAGE_SIZE EQU 4096 ; 4KB
ENT_NB EQU 1024 ; Number of entries in page directory and page tables

SECTION .boot
GLOBAL setup_paging
setup_paging:
	; Identity mapping the first 4MB
	MOV edi, page_tbl - KERNEL_START ; Physical address of page_tbl
	MOV esi, 0 ; We start the mapping from 0 for identity map
	MOV ecx, ENT_NB

	.map_entry:
	MOV edx, esi ; Memory frame base address
	OR edx, 00000011b ; Present and write flags
	MOV dword [edi], edx ; Set page table entry
	ADD edi, 4 ; Next entry in page_tbl
	ADD esi, PAGE_SIZE ; Next memory frame
	LOOP .map_entry

	; Points the physical address of the page_dir to the kernel
	MOV eax, (page_dir - KERNEL_START)
	MOV cr3, eax

	; Enable paging
	MOV eax, cr0
	OR eax, 0x80000000
	MOV cr0, eax

	RET

SECTION .data
ALIGN 4096
GlOBAL page_dir
GLOBAL page_tbl
; Set up paging directory statically at compile time
page_dir:
	DD (page_tbl - KERNEL_START) + 00000011b ; For identity mapping
	TIMES 768 - 1 DD 0
	; Map the first 4MB
	DD (page_tbl - KERNEL_START) + 00000011b ; Higher half mapping
	TIMES 256 - 1 DD 0
page_tbl:
	TIMES 1024 DD 0
