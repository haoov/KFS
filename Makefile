# Project
TARGET			:= kfs.iso
DIST_DIR		:= dist
BUILD_DIR		:= build
SRC_DIR			:= src
TARGET_DIR		:= targets
X86_64_SRC		:= $(SRC_DIR)/x86_64
X86_64_BUILD	:= $(BUILD_DIR)/x86_64
X86_64_DIST		:= $(DIST_DIR)/x86_64
X86_64_TARGET	:= $(TARGET_DIR)/x86_64

# Files
X86_64_LINKER	:= $(X86_64_TARGET)/linker.ld
X86_64_ASM_SRCS	:= $(wildcard $(X86_64_SRC)/boot/*.asm)
X86_64_ASM_OBJS	:= $(patsubst $(X86_64_SRC)/boot/%.asm,$(X86_64_BUILD)/%.o,$(X86_64_ASM_SRCS))

# Compiler
CC				:= x86_64-elf-gcc
LNK				:= x86_64-elf-ld
ASM				:= nasm

# Rules
build-x86_64 $(TARGET): $(X86_64_ASM_OBJS) | $(X86_64_DIST)
	$(LNK) -n -o $(X86_64_TARGET)/iso/boot/kernel.bin -T $(X86_64_LINKER) $(X86_64_ASM_OBJS)
	grub-mkrescue /usr/lib/grub/i386-pc -o $(X86_64_DIST)/$(TARGET) $(X86_64_TARGET)/iso

$(X86_64_BUILD)/%.o: $(X86_64_SRC)/boot/*.asm | $(X86_64_BUILD)
	$(ASM) -f elf64 $< -o $@

$(X86_64_BUILD) $(X86_64_DIST):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(DIST_DIR)

re: fclean all

.PHONY: all clean fclean re
