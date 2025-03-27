#ifndef PORTS_H
#define PORTS_H

#include "klib.h"

#define KB_STATUS_PORT	0x64
#define KB_CTRL_PORT	0x64
#define KB_DATA_PORT	0x60

void	outb(uint16_t port, uint8_t byte);
uint8_t	inb(uint16_t port);

#endif
