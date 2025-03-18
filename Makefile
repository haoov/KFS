# Project
TARGET		= kfs.iso
KERNEL		= kernel.bin
PROJ_DIR	= $(realpath $(CURDIR))
SRC_DIR		= $(PROJ_DIR)/src
OBJ_DIR		= $(PROJ_DIR)/objs
LDBUILD_DIR	= $(PROJ_DIR)/build_loader

# Files
SRCS		= $(wildcard $(SRC_DIR)/*.c)
OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LSCRIPT		= 

# Compiler
CC			= gcc
CFLAGS		= -m64 \
			  -fno-stack-protector \
			  -nostdlib \
			  -nodefaultlibs \
			  -ffreestanding \
			  -z max-page-size=0x1000 \
			  -mno-red-zone \
			  -mno-mmx \
			  -mno-sse \
			  -mno-sse2 \
			  -std=gnu99 \
			  -O2 \
			  -Wall \
			  -Wextra \
			  -Werror
LFLAGS		= -ffreestanding -O2 -nostdlib -lgcc

# Rules
all : $(TARGET)

$(TARGET) : $(KERNEL)
	grub-mkrescue -o $(TARGET).iso $(LDBUILD_DIR)/

$(KERNEL) : $(OBJS)
	$(CC) $(LFLAGS) -T $(LSCRIPT) $^ -o $@

compile : $(OBJS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) :
	mkdir -p $@

clean :
	rm -rf $(OBJS)
