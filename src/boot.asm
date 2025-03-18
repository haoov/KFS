%define ALIGN 1 << 0
%define MEMINFO 1 << 1
%define FLAGS ALIGN | MEMINFO
%define MAGIC 0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .text
bits 32
global _start
_start:
	mov dword [0xb8000], 0x2f4b2f4f
	hlt
