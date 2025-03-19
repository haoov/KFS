# Project
TARGET			:= kfs.iso
KERNEL			:= kernel.bin
X86_64_SRC_DIR	:= src/impl/x86_64
X86_64_OBJ_DIR	:= objs/x86_64

# Files
X86_64_LINKER		:= targets/x86_64/linker.ld
X86_64_ASM_SRCS		:= $(wildcard $(X86_64_SRC_DIR)/*.asm)
X86_64_ASM_OBJS		:= $(patsubst $(X86_64_SRC_DIR)/%.asm,$(X86_64_OBJ_DIR)/%.o,$(X86_64_ASM_SRCS))
X86_64KERNEL_BIN	:= ./targets/x86_64/iso/boot/kernel.bin

# Compiler
ASMC		:= nasm
CC			:= x86_64-elf-gcc
CFLAGS		:= -ffreestanding \
LFLAGS		:= -ffreestanding -nostdlib

# Rules
all : $(TARGET)

$(TARGET) : $(X86_64_ASM_OBJS)
	x86_64-elf-ld -n -o $(X86_64KERNEL_BIN) -T $(LINKER) $(LFLAGS) $^
	grub-mkrescue -o $@ ./targets/x86_64/iso

$(X86_64_OBJ_DIR)/%.o : $(X86_64_SRC_DIR)/%.asm | $(X86_64_OBJ_DIR)
	$(ASMC) -f elf64 $< -o $@

$(X86_64_OBJ_DIR) :
	mkdir -p $@

clean :
	rm -rf objs
	rm -rf $(X86_64KERNEL_BIN)

fclean : clean
	rm -rf $(TARGET)
