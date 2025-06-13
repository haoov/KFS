%define MAGIC 0xe85250d6
%define ARCH 0
%define TYPE 0
%define FLAGS 0

section .header
header_start:
	; Multiboot2 magic
	dd MAGIC
	dd ARCH
	dd header_end - header_start
	dd 0x100000000 - (MAGIC + ARCH + (header_end - header_start))

	; End tag
	dw TYPE
	dw FLAGS
	dd 8
header_end:
