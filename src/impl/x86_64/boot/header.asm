section .multiboot_header
	align 4 
	MAGIC equ 0x1BADB002
	FLAGS equ 0x0
	CHECKSUM equ -(MAGIC + FLAGS)

	dd MAGIC
	dd FLAGS
	dd CHECKSUM
