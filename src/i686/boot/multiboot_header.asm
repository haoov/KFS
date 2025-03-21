%define FLAGS 0x3 ; align + meminfo flags
%define MAGIC 0x1BADB002 ; multiboot magic number
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot_header
	align 8
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
