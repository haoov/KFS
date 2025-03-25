global start
extern kmain

section .text
bits 32

; add multiboot check

start:
	mov esp, stack_top
	call kmain
hlt_loop:
	hlt
	jmp hlt_loop

section .bss
align 4096
stack_bottom:
	resb	4096 * 4
stack_top:
