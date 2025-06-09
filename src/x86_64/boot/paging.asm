section .boot
bits 32

global setup_paging
global enable_paging

setup_paging:
	; Set up pml4 for identity mapping
	mov eax, pml3
	or eax, 3
	mov dword [pml4], eax
	mov dword [pml4 + 4], 0

	; Set up pml4 for higher half mapping
	mov eax, pml3_higher
	or eax, 3
	mov dword [pml4 + 511 * 8], eax
	mov dword [pml4 + 511 * 8 + 4], 0

	; Set up pml3 for identity mapping
	mov eax, pml2
	or eax, 3
	mov dword [pml3], eax
	mov dword [pml3 +4 ], 0

	; Set up pml3 for higher half mapping
	mov eax, pml2
	or eax, 3
	mov dword [pml3_higher + 510 * 8], eax
	mov dword [pml3_higher + 510 * 8 + 4], 0

	; Set up pml2 for identity mapping
	mov eax, pml1
	or eax, 3
	mov dword [pml2], eax
	mov dword [pml2 + 4], 0

	; Set up pml2 for higher half mapping
	mov eax, pml1_higher
	or eax, 3
	mov dword [pml2_higher], eax
	mov dword [pml2_higher + 4], 0

	; Identity map the first 2MB
	mov edi, pml1
	mov eax, 0; Start physical address at 0
	mov ecx, 512
	call map_pages

	; Higher half mapping the first 2MB
	mov edi, pml1_higher
	mov eax, 0
	mov ecx, 512
	call map_pages

	mov eax, pml4
	mov cr3, eax
	ret

map_pages:
	push eax
	push edi

	.map:
		or eax, 3
		mov dword [edi], eax
		mov dword [edi + 4], 0
		add eax, 4096
		add edi, 8
		loop .map

	pop edi
	pop eax
	ret

enable_paging:
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax
	ret

align 4096
pml1:
	times 1024 dd 0
pml2:
	times 1024 dd 0
pml3:
	times 1024 dd 0
pml4:
	times 1024 dd 0
pml1_higher:
	times 1024 dd 0
pml2_higher:
	times 1024 dd 0
pml3_higher:
	times 1024 dd 0
