SECTION .boot
EXTERN kmain
EXTERN setup_paging
GLOBAL _start
_start:
	CALL setup_paging
	JMP higher_half

SECTION .text
higher_half:
	MOV esp, stack_top
	PUSH ebx
	CALL kmain

hlt_loop:
	HLT
	JMP hlt_loop

SECTION .bss
ALIGN 4096
stack_bottom:
	RESB	4096 * 4
stack_top:
