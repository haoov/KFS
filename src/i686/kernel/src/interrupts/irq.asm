extern irq_handler
extern kprint_hex
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

section .text
bits 32

irq0:
	cli
	push dword 0
	push dword 32
	jmp irq_common_subr

irq1:
	cli
	push dword 0
	push dword 33
	jmp irq_common_subr

irq2:
	cli
	push dword 0
	push dword 34
	jmp irq_common_subr

irq3:
	cli
	push dword 0
	push dword 35
	jmp irq_common_subr

irq4:
	cli
	push dword 0
	push dword 36
	jmp irq_common_subr

irq5:
	cli
	push dword 0
	push dword 37
	jmp irq_common_subr

irq6:
	cli
	push dword 0
	push dword 38
	jmp irq_common_subr

irq7:
	cli
	push dword 0
	push dword 39
	jmp irq_common_subr

irq8:
	cli
	push dword 0
	push dword 40
	jmp irq_common_subr

irq9:
	cli
	push dword 0
	push dword 41
	jmp irq_common_subr

irq10:
	cli
	push dword 0
	push dword 42
	jmp irq_common_subr

irq11:
	cli
	push dword 0
	push dword 43
	jmp irq_common_subr

irq12:
	cli
	push dword 0
	push dword 44
	jmp irq_common_subr

irq13:
	cli
	push dword 0
	push dword 45
	jmp irq_common_subr

irq14:
	cli
	push dword 0
	push dword 46
	jmp irq_common_subr

irq15:
	cli
	push dword 0
	push dword 47
	jmp irq_common_subr

irq_common_subr:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

