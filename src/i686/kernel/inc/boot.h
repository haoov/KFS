#ifndef BOOT_H
#define BOOT_H

#include "klib.h"

typedef struct symbol_table_infos {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
} symbtab_inf_t __attribute__ ((packed));

typedef struct shdrtab_infos {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
} shdrtab_inf_t __attribute__ ((packed));

typedef struct module {
	uint32_t	mod_start;
	uint32_t	mod_end;
	uint32_t	string;
	uint32_t	reserved;
} mb_module_t __attribute__ ((packed));

typedef struct mmap_ent {
	uint32_t	size;
	uint32_t	base_addr_low;
	uint32_t	base_addr_high;
	uint32_t	lenght_low;
	uint32_t	length_high;
	uint32_t	type;
} mmap_ent_t __attribute__ ((packed));

#define MMAP_TYPE_AVAILABLE	1
#define MMAP_TYPE_ACPI		3
#define MMAP_TYPE_RESERVED	4
#define MMAP_TYPE_DEF_RAM	5

typedef struct multiboot_infos {
	uint32_t	flags;				// Presence and validity of fields

	uint32_t	mem_lower;			// Amount of lower memory < 1MB
	uint32_t	mem_upper;			// Amount of upper memory >= 1MB

	uint32_t	boot_device;		// Which BIOS disk image was booted

	uint32_t	cmdline;			// Physical address of cmd line str

	uint32_t	mods_count;			// Number of modules loaded
	uint32_t	mods_addr;			// Physical address of first module

	union {
		symbtab_inf_t	symtab;		// Symbol table of kernel file
		shdrtab_inf_t	shdrtab;	// Section header table of kernel file
	} syms;

	uint32_t	mmap_length;		// Length of memory map
	uint32_t	mmap_addr;			// Physical address of memory map

	uint32_t	drives_length;
	uint32_t	drives_addr;
	uint32_t	config_table;
	uint32_t	boot_loader_name;
	uint32_t	apm_table;
	uint32_t	vbe_control_info;
	uint32_t	vbe_mode_info;
	uint32_t	vbe_mode;
	uint32_t	vbe_interface_seg;
	uint32_t	vbe_interface_off;
	uint32_t	vbe_interface_len;
	uint32_t	framebuffer_addr;
	uint32_t	framebuffer_pitch;
	uint32_t	framebuffer_width;
	uint32_t	framebuffer_height;
	uint32_t	framebuffer_bpp;
	uint32_t	framebuffer_type;
	uint32_t	color_info;
} mb_inf_t __attribute__ ((packed));

#define MB_MEM_VALID(f)			(f & (1 << 0))
#define MB_BOOT_DEVICE_VALID(f)	(f & (1 << 1))
#define MB_CMDLINE_VALID(f)		(f & (1 << 2))
#define MB_MODS_VALID(f)		(f & (1 << 3))
#define MB_SYMTAB_VALID(f)		(f & (1 << 4))
#define MB_SHDRTAB_VALID(f)		(f & (1 << 5))
#define MB_MMAP_VALID(f)		(f & (1 << 6))

#endif
