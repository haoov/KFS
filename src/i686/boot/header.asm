FLAGS EQU 0x3 ; align + meminfo flags
MAGIC EQU 0x1BADB002 ; multiboot magic number
CHECKSUM EQU -(MAGIC + FLAGS)

SECTION .header
	ALIGN 4
	DD MAGIC
	DD FLAGS
	DD CHECKSUM
