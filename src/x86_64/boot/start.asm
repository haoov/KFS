section .boot
bits 32

extern kmain
extern setup_paging
extern setup_gdt
extern check_long_mode
extern enable_long_mode
extern enable_paging
global _start

_start:
	; Save multiboot info
	mov esi, ebx

	; Set up temporary stack (physical address)
	mov esp, 0x200000 - 0x1000  ; Use memory at 2MB-4KB as temp stack

	; Check for long mode support
	call check_long_mode

	call setup_gdt

	call setup_paging

	; Enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	call enable_long_mode

	call enable_paging

	; Jump to 64-bit code
	jmp 0x08:long_mode_start_phys

BITS 64
long_mode_start_phys:
	; Now we're in 64-bit mode, jump to higher half
	mov rax, higher_half
	jmp rax

SECTION .text
higher_half:
	; Set up segment registers
	mov ax, 0x10	; Data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Set up proper 64-bit stack
	mov rsp, stack_top

	; Call kernel main
	mov rdi, rsi	; Pass multiboot info as first argument

	call kmain

hlt_loop:
	hlt
	jmp hlt_loop

SECTION .bss
ALIGN 4096
stack_bottom:
	resb 4096 * 16
stack_top:
