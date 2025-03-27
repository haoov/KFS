global idt_load
extern idtp

section .text
bits 32

idt_load:
	lidt [idtp]
	ret
