#ifndef KERNEL_H 
#define KERNEL_H

#include "klib.h"
#include "intf.h"
#include "ports.h"
#include "gdt.h"
#include "idt.h"

struct regs
{
	uint32_t	gs, fs, es, ds;
	uint32_t	edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t	int_no, err_code;
	uint32_t	eip, cs, eflags, useresp, ss;
};

#include "isr.h"

#endif
