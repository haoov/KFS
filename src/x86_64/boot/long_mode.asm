section .boot
bits 32

global check_long_mode
global enable_long_mode

check_long_mode:
	; Check if CPUID is supported
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	xor eax, ecx
	jz .no_cpuid

	; Check for extended processor info
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_extended

	; Check for long mode
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode
	ret

	.no_extended:
		mov word [0xB8002], 0x0C45    ; Red 'E' at position 1
		hlt
		jmp $

	.no_cpuid:
		mov dword [0xB8000], 0x0C500C43
		hlt
		jmp $

	.no_long_mode:
		hlt
		jmp $

enable_long_mode:
	; Set long mode bit in EFER MSR
	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x00000100
	wrmsr
	ret

