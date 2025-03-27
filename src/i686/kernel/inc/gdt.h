#ifndef GDT_H
#define GDT_H

#include "klib.h"

#define ACC_PRESENT	0x80 // Segment is present in memory
#define ACC_R0		0x00 // Kernel privilege level
#define ACC_R3		0x60 // User privilege level
#define ACC_DESC	0x10 // Code / Data segment
#define ACC_EXEC	0x08 // Excecutable code segment
#define ACC_DATA_WR	0x02 // Writeable data segment
#define ACC_CODE_RD	0x02 // Readable code segment

#define FLAG_GRAN4K	0x80 // 4kb granularity
#define FLAG_32BIT	0x40 // 32 bit protected mode

#define MEM_LIMIT	0xFFFFFFFF // Maximum memory address

#define GDT_PHYSICAl_ADDR	0x00000800

struct gdt_entry
{
	uint16_t	limit_lo;
	uint16_t	base_lo;
	uint8_t		base_mid;
	uint8_t		access;
	uint8_t		gran;
	uint8_t		base_hi;
} __attribute__((packed));

struct gdt_ptr
{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));

extern void	gdt_flush(void);
void		gdt_install(void);

#endif
