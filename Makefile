# Project
TARGET		:= kfs.iso
KERNEL		:= kernel.bin
PROJ_DIR	:= $(realpath $(CURDIR))
SRC_DIR		:= $(PROJ_DIR)/src
OBJ_DIR		:= $(PROJ_DIR)/objs
TARGET_DIR	:= $(PROJ_DIR)/target

# Files
LINKER		:= $(TARGET_DIR)/x86_64/linker.ld
BOOT		:= $(SRC_DIR)/boot
SRCS		:= $(wildcard $(SRC_DIR)/*.c)
OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Compiler
ASMC		:= nasm
CC			:= gcc
CFLAGS		:=	-Wall \
				-Wextra \
				-Werror \
				-ffreestanding \
				-fno-builtin \
				-fno-stack-protector
LFLAGS		:= -ffreestanding -nostdlib

# Rules
all : $(TARGET)

$(TARGET) :
	$(ASMC) -f elf64 $(BOOT).asm -o $(BOOT).o
	$(CC) -T $(LINKER) $(LFLAGS) $(BOOT).o -o $(TARGET_DIR)/x86_64/iso/boot/kernel.bin
	grub-mkrescue -o $@ $(TARGET_DIR)/x86_64/iso
