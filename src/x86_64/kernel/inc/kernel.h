#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"
#include "idt.h"

void  halt_cpu(void);

void  kernel_panic(const char *msg, struct interrupt_frame *frame);
void print_debug(void);

#endif
