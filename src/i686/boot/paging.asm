SECTION .boot
GLOBAL setup_paging
setup_paging:
	MOV eax, (initial_page_dir - 0xC0000000)
	MOV cr3, eax

	MOV eax, cr4
	OR eax, 0x10
	MOV cr4, eax

	MOV eax, cr0
	OR eax, 0x80000000
	MOV cr0, eax

	RET

SECTION .data
ALIGN 4096
GlOBAL initial_page_dir
initial_page_dir:
	DD 10000011b
	TIMES 768 - 1 DD 0

	DD (0 << 22) | 10000011b
	DD (1 << 22) | 10000011b
	DD (2 << 22) | 10000011b
	DD (3 << 22) | 10000011b

	TIMES 256 - 4 DD 0
