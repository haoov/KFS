%define GDT_PRESENT (1 << 47)
%define GDT_CODE_DATA (1 << 44)
%define GDT_EXECUTABLE (1 << 43)
%define GDT_DC (1 << 42)
%define GDT_RW (1 << 41)
%define GDT_ACCESSED (1 << 40)
%define GDT_DB (1 << 54)
%define GDT_GRAN (1 << 55)
%define GDT_LONGMODE (1 << 53)

%define GDT_DPL0 (0 << 45)
%define GDT_DPL1 (1 << 45)
%define GDT_DPL2 (2 << 45)
%define GDT_DPL3 (3 << 45)

section .boot
bits 32

global setup_gdt

setup_gdt:
	mov dword [gdt_ptr + 2], gdt
	lgdt [gdt_ptr]
	ret
	
align 16
gdt:
	dq 0 ; Null descriptor
	dq GDT_PRESENT | GDT_CODE_DATA | GDT_RW | GDT_EXECUTABLE | GDT_LONGMODE | GDT_DPL0
	dq GDT_PRESENT | GDT_CODE_DATA | GDT_RW | GDT_DPL0

	dq GDT_PRESENT | GDT_CODE_DATA | GDT_RW | GDT_EXECUTABLE | GDT_LONGMODE | GDT_DPL3
	dq GDT_PRESENT | GDT_CODE_DATA | GDT_RW | GDT_DPL3

gdt_ptr:
	dw 39	; GDT size - 1
	dd 0 	; Will be filled with physical address
