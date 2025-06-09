section .text
bits 64

extern interrupt_handler

%macro SAVE_REGISTERS 0
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro RESTORE_REGISTERS 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro ISR_INT_NB 1
global isr%1
isr%1:
	push 0
	push %1
	jmp isr_common
%endmacro

%macro ISR_NO_INT_NB 1
global isr%1
isr%1:
	push %1
	jmp isr_common
%endmacro

%macro IRQ_SUB 1
global irq%1
irq%1:
	push 0
	push %1+32
	jmp isr_common
%endmacro

ISR_INT_NB 0 ; Divide by zero
ISR_INT_NB 1
ISR_INT_NB 2
ISR_INT_NB 3
ISR_INT_NB 4
ISR_INT_NB 5
ISR_INT_NB 6
ISR_INT_NB 7
ISR_NO_INT_NB 8
ISR_INT_NB 9
ISR_NO_INT_NB 10
ISR_NO_INT_NB 11
ISR_NO_INT_NB 12
ISR_NO_INT_NB 13
ISR_NO_INT_NB 14
ISR_INT_NB 15
ISR_INT_NB 16
ISR_INT_NB 17
ISR_INT_NB 18
ISR_INT_NB 19
ISR_INT_NB 20
ISR_INT_NB 21
ISR_INT_NB 22
ISR_INT_NB 23
ISR_INT_NB 24
ISR_INT_NB 25
ISR_INT_NB 26
ISR_INT_NB 27
ISR_INT_NB 28
ISR_INT_NB 29
ISR_INT_NB 30
ISR_INT_NB 31

IRQ_SUB 0
IRQ_SUB 1
IRQ_SUB 2
IRQ_SUB 3
IRQ_SUB 4
IRQ_SUB 5
IRQ_SUB 6
IRQ_SUB 7
IRQ_SUB 8
IRQ_SUB 9
IRQ_SUB 10
IRQ_SUB 11
IRQ_SUB 12
IRQ_SUB 13
IRQ_SUB 14
IRQ_SUB 15

isr_common:
	SAVE_REGISTERS

	mov rdi, rsp
	call interrupt_handler

	RESTORE_REGISTERS

	add rsp, 16
	iretq
